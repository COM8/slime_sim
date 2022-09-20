#pragma once

#include "opengl/SpeciesPreviewGlObject.hpp"
#include "sim/Simulation.hpp"
#include "sim/TickDurationHistory.hpp"
#include "sim/TickRate.hpp"
#include <cstddef>
#include <memory>
#include <epoxy/gl.h>
#include <gtkmm.h>
#include <gtkmm/glarea.h>

namespace ui::widgets {

class SpeciesPreviewWidget : public Gtk::GLArea {
 private:
    std::shared_ptr<sim::Simulation> simulation{sim::Simulation::get_instance()};
    size_t speciesIndex{0};
    bool speciesChanged{true};

    sim::TickDurationHistory fpsHistory{};
    sim::TickRate fps{};

    // OpenGL:
    GLint defaultFb{0};

    opengl::SpeciesPreviewGlObject speciesPreviewObj;

 public:
    bool enableUiUpdates{true};

    SpeciesPreviewWidget();

    [[nodiscard]] const sim::TickRate& get_fps() const;
    [[nodiscard]] const sim::TickDurationHistory& get_fps_history() const;

    void set_species_index(size_t speciesIndex);

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    bool on_render_handler(const Glib::RefPtr<Gdk::GLContext>& ctx);
    bool on_tick(const Glib::RefPtr<Gdk::FrameClock>& frameClock);
    void on_realized();
    void on_unrealized();
};
}  // namespace ui::widgets
