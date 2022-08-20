#include "SimulationWidget.hpp"
#include "logger/Logger.hpp"
#include "spdlog/fmt/bundled/core.h"
#include "spdlog/spdlog.h"
#include "ui/widgets/opengl/Slime.hpp"
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <limits>
#include <string>
#include <bits/chrono.h>
#include <epoxy/gl.h>
#include <epoxy/gl_generated.h>
#include <fmt/core.h>
#include <glibconfig.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/gesturezoom.h>

namespace ui::widgets {
SimulationWidget::SimulationWidget() : slimeObj(slimes, RESOLUTION_X, RESOLUTION_Y),
                                       screenSquareObj(RESOLUTION_X, RESOLUTION_Y),
                                       slimeFrameBuffer(RESOLUTION_X, RESOLUTION_Y) {
    prep_widget();
}

void SimulationWidget::set_zoom_factor(float zoomFactor) {
    assert(zoomFactor > 0);

    this->zoomFactor = zoomFactor;
    float widthF = static_cast<float>(RESOLUTION_X) * this->zoomFactor;
    int width = static_cast<int>(widthF);
    float heightF = static_cast<float>(RESOLUTION_Y) * this->zoomFactor;
    int height = static_cast<int>(heightF);
    glArea.set_size_request(width, height);
    glArea.queue_draw();
}

void SimulationWidget::prep_widget() {
    set_expand();

    glArea.signal_render().connect(sigc::mem_fun(*this, &SimulationWidget::on_render_handler), true);
    glArea.signal_realize().connect(sigc::mem_fun(*this, &SimulationWidget::on_realized));
    glArea.signal_unrealize().connect(sigc::mem_fun(*this, &SimulationWidget::on_unrealized));
    glArea.add_tick_callback(sigc::mem_fun(*this, &SimulationWidget::on_tick));

    glArea.set_auto_render();
    glArea.set_size_request(RESOLUTION_X, RESOLUTION_Y);
    set_child(glArea);
    screenSquareObj.set_glArea(&glArea);
}

void SimulationWidget::init_slimes() {
    for (size_t i = 0; i < NUM_SLIMES; i++) {
        slimes->emplace_back(opengl::Slime(
            opengl::Rgba::random_color(),
            opengl::Vec4U::random_vec(),
            opengl::Vec2::random_vec(0, RESOLUTION_X, 0, RESOLUTION_Y),
            opengl::Vec2::random_vec(0, RESOLUTION_X, 0, RESOLUTION_Y),
            0));
    }
}

const utils::TickRate& SimulationWidget::get_fps() const {
    return fps;
}

const utils::TickDurationHistory& SimulationWidget::get_fps_history() const {
    return fpsHistory;
}

float SimulationWidget::get_zoom_factor() const {
    return zoomFactor;
}

void SimulationWidget::set_blur(bool blur) {
    this->blur = blur;
}

//-----------------------------Events:-----------------------------
bool SimulationWidget::on_render_handler(const Glib::RefPtr<Gdk::GLContext>& /*ctx*/) {
    std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();

    try {
        glArea.throw_if_error();

        // Get default frame buffer since in GTK it is not always 0:
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFb);

        // Backup the old viewport:
        std::array<int, 4> viewPort{};
        glGetIntegerv(GL_VIEWPORT, viewPort.data());

        // Draw:
        glDisable(GL_DEPTH_TEST);

        // 1.0 Draw slimes to buffer:
        slimeFrameBuffer.bind();
        // 1.1 Blur old slime image:
        if (blur) {
            // blurObject.render();
        } else {
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            GLERR;
        }

        // 1.2 Draw slimes:
        slimeObj.render();

        // 4.0 Draw to screen:
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFb);
        GLERR;

        // Fix view port so it does not only show values in range [-1,0]:
        glViewport(viewPort[0], viewPort[1], viewPort[2], viewPort[3]);

        // 4.1 Clear the old screen:
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        GLERR;

        // 4.2 Draw texture from frame buffer:
        screenSquareObj.render();
    } catch (const Gdk::GLError& gle) {
        SPDLOG_ERROR("An error occurred in the render callback of the GLArea: {} - {} - {}", gle.domain(), gle.code(), gle.what());
    }

    std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
    fpsHistory.add_time(frameEnd - frameStart);

    // FPS counter:
    fps.tick();
    return false;
}

bool SimulationWidget::on_tick(const Glib::RefPtr<Gdk::FrameClock>& /*frameClock*/) {
    if (this->enableUiUpdates) {
        glArea.queue_draw();
    }
    return true;
}

void SimulationWidget::on_realized() {
    init_slimes();

    glArea.make_current();
    try {
        glArea.throw_if_error();

        slimeFrameBuffer.init();

        // Get default frame buffer since in GTK it is not always 0:
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFb);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFb);

        blurObject.set_texture_size(slimeFrameBuffer.get_texture_size_x(), slimeFrameBuffer.get_texture_size_y());
        blurObject.init();
        blurObject.bind_texture(slimeFrameBuffer.get_texture());
        slimeObj.init();
        screenSquareObj.bind_texture(slimeFrameBuffer.get_texture());
        screenSquareObj.init();
    } catch (const Gdk::GLError& gle) {
        SPDLOG_ERROR("An error occurred making the context current during realize: {} - {} - {}", gle.domain(), gle.code(), gle.what());
    }
}

void SimulationWidget::on_unrealized() {
    glArea.make_current();
    try {
        glArea.throw_if_error();

        blurObject.cleanup();
        slimeObj.cleanup();
        screenSquareObj.cleanup();

        slimeFrameBuffer.cleanup();
    } catch (const Gdk::GLError& gle) {
        SPDLOG_ERROR("An error occurred deleting the context current during unrealize: {} - {} - {}", gle.domain(), gle.code(), gle.what());
    }
}
}  // namespace ui::widgets
