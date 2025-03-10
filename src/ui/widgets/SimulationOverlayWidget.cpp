#include "SimulationOverlayWidget.hpp"
#include "ui/widgets/SimulationWidget.hpp"
#include <cassert>
#include <format>
#include <string>
#include <bits/chrono.h>
#include <gtkmm/enums.h>

namespace ui::widgets {
SimulationOverlayWidget::SimulationOverlayWidget(SimulationWidget* simWidget) : simWidget(simWidget) {
    prep_widget();
}

void SimulationOverlayWidget::prep_widget() {
    drawingArea.set_draw_func(sigc::mem_fun(*this, &SimulationOverlayWidget::on_draw_handler));
    drawingArea.add_tick_callback(sigc::mem_fun(*this, &SimulationOverlayWidget::on_tick));
    add_overlay(drawingArea);
    set_child(speciesPreviewWidget);
    set_species_preview_widget_size();

    // Disable focus since this widget is just a overlay for the actual simulation:
    set_can_target(false);
    set_can_focus(false);
}

void SimulationOverlayWidget::draw_text(const std::string& text, const Cairo::RefPtr<Cairo::Context>& ctx, double x, double y) {
    ctx->save();
    Pango::FontDescription font;
    font.set_weight(Pango::Weight::BOLD);
    Glib::RefPtr<Pango::Layout> layout = create_pango_layout(text);
    layout->set_font_description(font);
    ctx->move_to(x, y);
    layout->add_to_cairo_context(ctx);
    Gdk::RGBA foreground = get_style_context()->get_color();
    ctx->set_source_rgba(foreground.get_red(), foreground.get_green(), foreground.get_blue(), foreground.get_alpha());
    ctx->fill_preserve();
    ctx->set_source_rgba(1 - foreground.get_red(), 1 - foreground.get_green(), 1 - foreground.get_blue(), foreground.get_alpha());
    ctx->set_line_width(0.3);
    ctx->stroke();
    ctx->restore();
}

void SimulationOverlayWidget::set_debug_overlay_enabled(bool enableDebugOverlay) {
    this->enableDebugOverlay = enableDebugOverlay;
}

void SimulationOverlayWidget::set_species_preview_overlay_enabled(bool enableSpeciesPreviewOverlay) {
    speciesPreviewWidget.set_visible(enableSpeciesPreviewOverlay);
}

void SimulationOverlayWidget::set_species_index(size_t speciesIndex) {
    speciesPreviewWidget.set_species_index(speciesIndex);
}

void SimulationOverlayWidget::set_species_preview_widget_size() {
    // TODO: Subscribe to size changes and adopt based on the size
    /*static constexpr int MAX_SIZE = 200;

    int sizeRequest = get_width() > get_height() ? static_cast<int>(std::round(static_cast<double>(get_height()) * 0.1)) : static_cast<int>(std::round(static_cast<double>(get_width()) * 0.1));
    sizeRequest = std::min(sizeRequest, MAX_SIZE);

    speciesPreviewWidget.set_size_request(sizeRequest, sizeRequest);*/

    speciesPreviewWidget.set_size_request(200, 200);
}

//-----------------------------Events:-----------------------------
void SimulationOverlayWidget::on_draw_handler(const Cairo::RefPtr<Cairo::Context>& ctx, int /*width*/, int /*height*/) {
    if (!enableDebugOverlay) {
        return;
    }
    assert(simWidget);
    assert(simulation);

    // double tps = simulator->get_tps().get_ticks();
    // std::string tpsTime = simulator->get_tps_history().get_avg_time_str();
    // std::string updateTickTime = simulator->get_update_tick_history().get_avg_time_str();
    // std::string collisionDetectionTickTime = simulator->get_collision_detection_tick_history().get_avg_time_str();

    double fps = simWidget->get_fps().get_ticks();
    std::string fpsTime = simWidget->get_fps_history().get_avg_time_str();

    std::locale local("en_US.UTF-8");
    // std::string stats = std::format("TPS: {:.2f}\nTick Time: {} (Update: {}, Collision: {})\n", tps, tpsTime, updateTickTime, collisionDetectionTickTime);
    std::string stats;
    stats += std::format("FPS: {:.2f}\nFrame Time: {}\n", fps, fpsTime);
    stats += std::format(local, "Slimes: {:L}, Species: {:L}\n", simulation->get_slimes()->size(), simulation->get_species()->size());
    stats += std::format("Zoom: {}\n", simWidget->get_zoom_factor());
    stats += std::format(local, "Render Resolution: {:L}x{:L}\n\n", simulation->get_width(), simulation->get_height());

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    stats += std::format("Vendor: {}\nModel: {}\n\n", reinterpret_cast<const char*>(SimulationWidget::get_device_vendor_name()), reinterpret_cast<const char*>(SimulationWidget::get_device_name()));

    stats += "F - Toggle full screen\n";
    stats += "P - Toggle simulation running\n";
    stats += "T - Toggle title\n";
    draw_text(stats, ctx, 5, 5);
}

bool SimulationOverlayWidget::on_tick(const Glib::RefPtr<Gdk::FrameClock>& /*frameClock*/) {
    drawingArea.queue_draw();
    return true;
}
}  // namespace ui::widgets
