#pragma once

#include "opengl/BlurGlObject.hpp"
#include "opengl/ScreenSquareGlObject.hpp"
#include "opengl/SlimeGlObject.hpp"
#include "opengl/fb/SlimeFrameBuffer.hpp"
#include "sim/Simulation.hpp"
#include "sim/TickDurationHistory.hpp"
#include "sim/TickRate.hpp"
#include <array>
#include <memory>
#include <epoxy/gl.h>
#include <gtkmm.h>
#include <gtkmm/glarea.h>
#include <gtkmm/scrolledwindow.h>

namespace ui::widgets {
constexpr GLsizei NUM_SLIMES = 50000;

class SimulationWidget : public Gtk::ScrolledWindow {
 private:
    std::shared_ptr<sim::Simulation> simulation{sim::Simulation::get_instance()};

    sim::TickDurationHistory fpsHistory{};
    sim::TickRate fps{};

    // OpenGL:
    GLint defaultFb{0};

    opengl::SlimeGlObject slimeObj{};
    opengl::ScreenSquareGlObject screenSquareObj;
    opengl::BlurGlObject blurObject{};

    opengl::fb::SlimeFrameBuffer slimeFrameBuffer;

    Gtk::GLArea glArea;
    float zoomFactor{1};
    uint32_t speciesIndex{0};
    uint32_t speciedAddEnabled{false};

 public:
    bool enableUiUpdates{true};

    SimulationWidget();

    [[nodiscard]] const sim::TickRate& get_fps() const;
    [[nodiscard]] const sim::TickDurationHistory& get_fps_history() const;

    static const unsigned char* get_device_vendor_name();
    static const unsigned char* get_device_name();

    void set_zoom_factor(float zoomFactor);
    [[nodiscard]] float get_zoom_factor() const;
    void set_species_index(uint32_t speciesIndex);
    void set_species_add_enabled(bool speciedAddEnabled);

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    bool on_render_handler(const Glib::RefPtr<Gdk::GLContext>& ctx);
    bool on_tick(const Glib::RefPtr<Gdk::FrameClock>& frameClock);
    void on_realized();
    void on_unrealized();
    void on_glArea_clicked(int nPress, double x, double y);
};
}  // namespace ui::widgets
