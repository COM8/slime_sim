#pragma once

#include "SimulationWidget.hpp"
#include "SpeciesPreviewCairoWidget.hpp"
#include "sim/Simulation.hpp"
#include <memory>
#include <gtkmm.h>

namespace ui::widgets {
class SimulationOverlayWidget : public Gtk::Overlay {
 private:
    SimulationWidget* simWidget{nullptr};
    SpeciesPreviewCairoWidget speciesPreviewWidget{};
    bool enableDebugOverlay{true};

    Gtk::DrawingArea drawingArea{};

    std::shared_ptr<sim::Simulation> simulation{sim::Simulation::get_instance()};

 public:
    explicit SimulationOverlayWidget(SimulationWidget* simWidget);

    void set_debug_overlay_enabled(bool enableDebugOverlay);
    void set_species_preview_overlay_enabled(bool enableSpeciesPreviewOverlay);
    void set_species_index(size_t speciesIndex);

 protected:
    void on_size_allocate(Gtk::Allocation& allocation);

 private:
    void prep_widget();

    void draw_text(const std::string& text, const Cairo::RefPtr<Cairo::Context>& ctx, double x, double y);
    void set_species_preview_widget_size();

    //-----------------------------Events:-----------------------------
    void on_draw_handler(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height);
    bool on_tick(const Glib::RefPtr<Gdk::FrameClock>& frameClock);
    void on_size_changed();
};
}  // namespace ui::widgets
