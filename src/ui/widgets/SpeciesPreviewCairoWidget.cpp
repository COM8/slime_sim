#include "SpeciesPreviewCairoWidget.hpp"
#include "sim/Simulation.hpp"
#include "sim/Species.hpp"
#include "sim/Types.hpp"
#include <cmath>
#include <numbers>
#include <gtkmm/enums.h>

namespace ui::widgets {
SpeciesPreviewCairoWidget::SpeciesPreviewCairoWidget() {
    prep_widget();
}

void SpeciesPreviewCairoWidget::prep_widget() {
    set_draw_func(sigc::mem_fun(*this, &SpeciesPreviewCairoWidget::on_draw_handler));
    add_tick_callback(sigc::mem_fun(*this, &SpeciesPreviewCairoWidget::on_tick));
    set_can_target(false);
    set_can_focus(false);

    set_halign(Gtk::Align::END);
    set_valign(Gtk::Align::END);
    set_margin(10);

    set_overflow(Gtk::Overflow::HIDDEN);
    add_css_class("card");
}

void SpeciesPreviewCairoWidget::set_species_index(size_t speciesIndex) {
    this->speciesIndex = speciesIndex;
    queue_draw();
}

sim::Vec2 SpeciesPreviewCairoWidget::calc_sensor_offset_center(sim::Species& species, float angel) {
    const double rad = static_cast<float>(angel * (std::numbers::pi / 180.0));
    const double sizeCorrection = ((species.sensorOffsetDst) + static_cast<float>(species.sensorSize));

    return sim::Vec2{
        static_cast<float>((sizeCorrection * std::sin(rad)) + sizeCorrection),
        static_cast<float>((sizeCorrection * std::cos(rad)) + sizeCorrection)};
}

//-----------------------------Events:-----------------------------
void SpeciesPreviewCairoWidget::on_draw_handler(const Cairo::RefPtr<Cairo::Context>& ctx, int width, int height) const {
    sim::Species& species = sim::Simulation::get_instance()->get_species()->at(speciesIndex);

    ctx->save();

    // Background:
    ctx->rectangle(0, 0, width, height);
    ctx->set_source_rgba(0.1, 0.1, 0.1, 0.6);
    ctx->fill();

    // Slime:
    const float padding = 10;
    const float totalWidth = 1 + (species.sensorOffsetDst * 2) + (static_cast<float>(species.sensorSize) * 2);
    const float totalWidthFactor = (static_cast<float>(width) - (padding * 2)) / totalWidth;

    const float wHalf = static_cast<float>(width) / 2;
    const float hHalf = static_cast<float>(height) / 2;

    const float actSlimeSize = totalWidthFactor * 1;  // *1 since a slime is one pixel in size
    int offsetX = static_cast<int>(std::round(wHalf - (actSlimeSize / 2)));
    int offsetY = static_cast<int>(std::round(hHalf - (actSlimeSize / 2)));
    ctx->rectangle(offsetX, offsetY, actSlimeSize, actSlimeSize);  // *1 since a slime is one pixel in size
    ctx->set_source_rgba(species.color.r, species.color.g, species.color.b, species.color.a);
    ctx->fill();

    // Sensor:
    const float actSensorSize = static_cast<float>(species.sensorSize) * totalWidthFactor;
    const float actSensorSizeHalf = (actSensorSize / 2);

    // Top:
    sim::Vec2 top = calc_sensor_offset_center(species, 180);
    top.x *= totalWidthFactor;
    top.y *= totalWidthFactor;

    ctx->rectangle(padding + top.x - actSensorSizeHalf, top.y + padding - actSensorSizeHalf, actSensorSize, actSensorSize);
    ctx->set_source_rgba(1 - species.color.r, 1 - species.color.g, 1 - species.color.b, species.color.a);
    ctx->fill();

    // Left:
    sim::Vec2 left = calc_sensor_offset_center(species, 270 + species.sensorAngleDegrees);
    left.x *= totalWidthFactor;
    left.y *= totalWidthFactor;

    ctx->rectangle(padding + left.x - actSensorSizeHalf, left.y + padding - actSensorSizeHalf, actSensorSize, actSensorSize);
    ctx->set_source_rgba(1 - species.color.r, 1 - species.color.g, 1 - species.color.b, species.color.a);
    ctx->fill();

    // Right:
    sim::Vec2 right = calc_sensor_offset_center(species, 90 - species.sensorAngleDegrees);
    right.x *= totalWidthFactor;
    right.y *= totalWidthFactor;

    ctx->rectangle(padding + right.x - actSensorSizeHalf, right.y + padding - actSensorSizeHalf, actSensorSize, actSensorSize);
    ctx->set_source_rgba(1 - species.color.r, 1 - species.color.g, 1 - species.color.b, species.color.a);
    ctx->fill();

    ctx->restore();
}

bool SpeciesPreviewCairoWidget::on_tick(const Glib::RefPtr<Gdk::FrameClock>& /*frameClock*/) {
    queue_draw();
    return true;
}
}  // namespace ui::widgets