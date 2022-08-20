#pragma once

#include "SimulationWidget.hpp"
#include <memory>
#include <gtkmm.h>

namespace ui::widgets {
class SimulationOverlayWidget : public Gtk::DrawingArea {
 private:
    SimulationWidget* simWidget{nullptr};
    bool enableDebugOverlay{true};

 public:
    explicit SimulationOverlayWidget(SimulationWidget* simWidget);

    void set_debug_overlay_enabled(bool enableDebugOverlay);

 private:
    void prep_widget();

    void draw_text(const std::string& text, const Cairo::RefPtr<Cairo::Context>& ctx, double x, double y);

    //-----------------------------Events:-----------------------------
    void on_draw_handler(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height);
    bool on_tick(const Glib::RefPtr<Gdk::FrameClock>& frameClock);
};
}  // namespace ui::widgets
