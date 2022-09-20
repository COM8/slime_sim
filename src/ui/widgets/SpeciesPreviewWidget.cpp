#include "SpeciesPreviewWidget.hpp"
#include "logger/Logger.hpp"
#include "opengl/utils/Utils.hpp"
#include <cassert>
#include <gtkmm/enums.h>

namespace ui::widgets {
SpeciesPreviewWidget::SpeciesPreviewWidget() {
    prep_widget();
}

void SpeciesPreviewWidget::prep_widget() {
    set_expand();

    signal_render().connect(sigc::mem_fun(*this, &SpeciesPreviewWidget::on_render_handler), true);
    signal_realize().connect(sigc::mem_fun(*this, &SpeciesPreviewWidget::on_realized));
    signal_unrealize().connect(sigc::mem_fun(*this, &SpeciesPreviewWidget::on_unrealized));
    add_tick_callback(sigc::mem_fun(*this, &SpeciesPreviewWidget::on_tick));

    set_auto_render();
    set_size_request(200, 200);
    set_halign(Gtk::Align::END);
    set_valign(Gtk::Align::END);
    set_margin(10);
}

const sim::TickRate& SpeciesPreviewWidget::get_fps() const {
    return fps;
}

const sim::TickDurationHistory& SpeciesPreviewWidget::get_fps_history() const {
    return fpsHistory;
}

void SpeciesPreviewWidget::set_species_index(size_t speciesIndex) {
    if (this->speciesIndex != speciesIndex) {
        speciesChanged = true;
        this->speciesIndex = speciesIndex;
    }
}

//-----------------------------Events:-----------------------------
bool SpeciesPreviewWidget::on_render_handler(const Glib::RefPtr<Gdk::GLContext>& /*ctx*/) {
    assert(simulation);
    std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();

    try {
        throw_if_error();

        // Get default frame buffer since in GTK it is not always 0:
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFb);

        // Backup the old viewport:
        std::array<int, 4> viewPort{};
        glGetIntegerv(GL_VIEWPORT, viewPort.data());

        // Draw:
        if (speciesChanged) {
            speciesChanged = false;
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            GLERR;
        }

        if (simulation->get_species()->size() <= speciesIndex) {
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            GLERR;
        } else {
            glDisable(GL_DEPTH_TEST);
            speciesPreviewObj.render();
        }

        // Fix view port so it does not only show values in range [-1,0]:
        glViewport(viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
    } catch (const Gdk::GLError& gle) {
        SPDLOG_ERROR("An error occurred in the render callback of the GLArea: {} - {} - {}", gle.domain(), gle.code(), gle.what());
    }

    std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
    fpsHistory.add_time(frameEnd - frameStart);

    // FPS counter:
    fps.tick();
    return false;
}

bool SpeciesPreviewWidget::on_tick(const Glib::RefPtr<Gdk::FrameClock>& /*frameClock*/) {
    if (this->enableUiUpdates) {
        queue_draw();
    }
    return true;
}

void SpeciesPreviewWidget::on_realized() {
    make_current();
    try {
        throw_if_error();

        speciesPreviewObj.init();

        // Get default frame buffer since in GTK it is not always 0:
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFb);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFb);

    } catch (const Gdk::GLError& gle) {
        SPDLOG_ERROR("An error occurred making the context current during realize: {} - {} - {}", gle.domain(), gle.code(), gle.what());
    }
}

void SpeciesPreviewWidget::on_unrealized() {
    make_current();
    try {
        throw_if_error();

        speciesPreviewObj.cleanup();
    } catch (const Gdk::GLError& gle) {
        SPDLOG_ERROR("An error occurred deleting the context current during unrealize: {} - {} - {}", gle.domain(), gle.code(), gle.what());
    }
}
}  // namespace ui::widgets
