#pragma once

#include "sim/Simulation.hpp"
#include <memory>
#include <gtkmm/drawingarea.h>

namespace ui::widgets {
class SpeciesPreviewCairoWidget : public Gtk::DrawingArea {
 private:
    std::shared_ptr<sim::Simulation> simulation{sim::Simulation::get_instance()};
    size_t speciesIndex{0};

 public:
    SpeciesPreviewCairoWidget();
    SpeciesPreviewCairoWidget(SpeciesPreviewCairoWidget&&) = delete;
    SpeciesPreviewCairoWidget(const SpeciesPreviewCairoWidget&) = delete;
    SpeciesPreviewCairoWidget& operator=(SpeciesPreviewCairoWidget&&) = delete;
    SpeciesPreviewCairoWidget& operator=(const SpeciesPreviewCairoWidget&) = delete;
    ~SpeciesPreviewCairoWidget() override = default;

    void set_species_index(size_t speciesIndex);

 private:
    void prep_widget();
    static sim::Vec2 calc_sensor_offset_center(sim::Species& species, float angel);

    //-----------------------------Events:-----------------------------
    void on_draw_handler(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height) const;
    bool on_tick(const Glib::RefPtr<Gdk::FrameClock>& frameClock);
};
}  // namespace ui::widgets
