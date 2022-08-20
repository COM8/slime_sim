#pragma once

#include "opengl/BlurGlObject.hpp"
#include "opengl/ScreenSquareGlObject.hpp"
#include "opengl/Slime.hpp"
#include "opengl/SlimeGlObject.hpp"
#include "opengl/fb/SlimeFrameBuffer.hpp"
#include "utils/TickDurationHistory.hpp"
#include "utils/TickRate.hpp"
#include <array>
#include <memory>
#include <epoxy/gl.h>
#include <gtkmm.h>
#include <gtkmm/glarea.h>
#include <gtkmm/scrolledwindow.h>

namespace ui::widgets {
constexpr GLsizei RESOLUTION_X = 8192;
constexpr GLsizei RESOLUTION_Y = 8192;

constexpr GLsizei NUM_SLIMES = 8192;

class SimulationWidget : public Gtk::ScrolledWindow {
 private:
    std::shared_ptr<std::vector<opengl::Slime>> slimes{std::make_shared<std::vector<opengl::Slime>>()};

    utils::TickDurationHistory fpsHistory{};
    utils::TickRate fps{};

    // OpenGL:
    GLint defaultFb{0};

    opengl::SlimeGlObject slimeObj;
    opengl::ScreenSquareGlObject screenSquareObj;
    opengl::BlurGlObject blurObject{};

    opengl::fb::SlimeFrameBuffer slimeFrameBuffer;
    bool blur{false};

    Gtk::GLArea glArea;
    float zoomFactor{1};

 public:
    bool enableUiUpdates{true};

    SimulationWidget();

    [[nodiscard]] const utils::TickRate& get_fps() const;
    [[nodiscard]] const utils::TickDurationHistory& get_fps_history() const;

    void set_zoom_factor(float zoomFactor);
    [[nodiscard]] float get_zoom_factor() const;

    void set_blur(bool blur);

 private:
    void prep_widget();
    void init_slimes();

    //-----------------------------Events:-----------------------------
    bool on_render_handler(const Glib::RefPtr<Gdk::GLContext>& ctx);
    bool on_tick(const Glib::RefPtr<Gdk::FrameClock>& frameClock);
    void on_realized();
    void on_unrealized();
};
}  // namespace ui::widgets
