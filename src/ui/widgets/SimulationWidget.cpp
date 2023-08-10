#include "SimulationWidget.hpp"
#include "logger/Logger.hpp"
#include "sim/Species.hpp"
#include "spdlog/fmt/bundled/core.h"
#include "spdlog/spdlog.h"
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
SimulationWidget::SimulationWidget() : screenSquareObj(static_cast<float>(simulation->get_width()), static_cast<float>(simulation->get_height())),
                                       slimeFrameBuffer(static_cast<GLsizei>(simulation->get_width()), static_cast<GLsizei>(simulation->get_height())) {
    prep_widget();
}

void SimulationWidget::set_zoom_factor(float zoomFactor) {
    assert(zoomFactor > 0);

    this->zoomFactor = zoomFactor;
    float widthF = static_cast<float>(simulation->get_width()) * this->zoomFactor;
    int width = static_cast<int>(widthF);
    float heightF = static_cast<float>(simulation->get_height()) * this->zoomFactor;
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
    glArea.set_size_request(static_cast<int>(simulation->get_width()), static_cast<int>(simulation->get_height()));
    set_child(glArea);
    screenSquareObj.set_glArea(&glArea);

    Glib::RefPtr<Gtk::GestureClick> clickGesture = Gtk::GestureClick::create();
    clickGesture->set_button(GDK_BUTTON_PRIMARY);
    clickGesture->signal_pressed().connect(sigc::mem_fun(*this, &SimulationWidget::on_glArea_clicked));
    glArea.add_controller(clickGesture);
}

const sim::TickRate& SimulationWidget::get_fps() const {
    return fps;
}

const sim::TickDurationHistory& SimulationWidget::get_fps_history() const {
    return fpsHistory;
}

float SimulationWidget::get_zoom_factor() const {
    return zoomFactor;
}

void SimulationWidget::set_species_index(uint32_t speciesIndex) {
    this->speciesIndex = speciesIndex;
}

void SimulationWidget::set_species_add_enabled(bool speciedAddEnabled) {
    this->speciedAddEnabled = speciedAddEnabled;
}

const unsigned char* SimulationWidget::get_device_vendor_name() {
    return glGetString(GL_VENDOR);
}

const unsigned char* SimulationWidget::get_device_name() {
    return glGetString(GL_RENDERER);
}

//-----------------------------Events:-----------------------------
bool SimulationWidget::on_render_handler(const Glib::RefPtr<Gdk::GLContext>& /*ctx*/) {
    assert(simulation);
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

        if (simulation->is_running()) {
            // Check if we have to change the color:
            if (simulation->is_color_change_enabled()) {
                sinceLastReset += std::chrono::high_resolution_clock::now() - lastFrame;

                if (std::chrono::floor<std::chrono::seconds>(sinceLastReset.time_since_epoch()) > resetColorTimer) {
                    sinceLastReset = std::chrono::high_resolution_clock::time_point{};
                    SPDLOG_INFO("Color change trigger hit. Changing color...");
                    for (sim::Species& species : *simulation->get_species()) {
                        species.color = sim::Rgba::random_color();
                    }
                    simulation->set_species_need_sync(true);
                }
            }
            else {
                sinceLastReset = std::chrono::high_resolution_clock::time_point{};
            }

            // 1.0 Draw slimes to buffer:
            slimeFrameBuffer.bind();
            // 1.1 Blur old slime image:
            if (simulation->is_blur_enabled()) {
                blurObject.render();
            } else {
                glClearColor(0, 0, 0, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                GLERR;
            }
            // 1.2 Draw slimes:
            slimeObj.render();
        }
        lastFrame = std::chrono::high_resolution_clock::now();

        // 2.0 Draw to screen:
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
        slimeObj.set_slime_texture(slimeFrameBuffer.get_texture());
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

void SimulationWidget::on_glArea_clicked(int /*nPress*/, double x, double y) {
    if (!speciedAddEnabled) {
        return;
    }

    // Invert since coordinates are inverted on the map:
    y = glArea.get_height() - y;

    assert(simulation);
    x *= static_cast<double>(simulation->get_width()) / glArea.get_width();
    y *= static_cast<double>(simulation->get_height()) / glArea.get_height();
    simulation->add_slimes(static_cast<float>(x), static_cast<float>(y), 10, speciesIndex);
}
}  // namespace ui::widgets
