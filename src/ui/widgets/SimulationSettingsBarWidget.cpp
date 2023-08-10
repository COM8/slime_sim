#include "SimulationSettingsBarWidget.hpp"
#include "sim/Types.hpp"
#include "ui/widgets/SimulationWidget.hpp"
#include <cassert>
#include <cstdint>
#include <gdkmm/display.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/box.h>
#include <gtkmm/enums.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/image.h>

namespace ui::widgets {
SimulationSettingsBarWidget::SimulationSettingsBarWidget(SimulationWidget* simWidget, SimulationOverlayWidget* simOverlayWidget) : Gtk::Box(Gtk::Orientation::HORIZONTAL),
                                                                                                                                   simWidget(simWidget),
                                                                                                                                   simOverlayWidget(simOverlayWidget),
                                                                                                                                   speciesTurnSpeedBtn(0, 0.5, 0.005),
                                                                                                                                   speciesSensorAngleDegreesBtn(0, 90, 1),
                                                                                                                                   speciesSensorSizeBtn(0, 10, 1),
                                                                                                                                   speciesSensorOffsetBtn(0, 100, 1) {
    prep_widget();
}

void SimulationSettingsBarWidget::prep_widget() {
    assert(simulation);

    mainBox.add_css_class("linked");
    mainBox.set_margin_start(10);
    append(mainBox);

    zoomBox.add_css_class("linked");
    zoomBox.set_margin_start(10);
    append(zoomBox);

    miscBox.add_css_class("linked");
    miscBox.set_margin_start(10);
    append(miscBox);

    speciesBox.add_css_class("linked");
    speciesBox.set_margin_start(10);
    append(speciesBox);

    simulateTBtn.property_active().signal_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_simulate_toggled));
    simulateTBtn.set_active(simulation->is_running());
    simulateTBtn.set_icon_name(simulation->is_running() ? "pause-large-symbolic" : "play-large-symbolic");
    simulateTBtn.set_tooltip_text("Enable simulation");
    simulateTBtn.add_css_class("suggested-action");
    mainBox.append(simulateTBtn);

    renderTBtn.property_active().signal_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_render_toggled));
    renderTBtn.set_active();
    renderTBtn.set_icon_name("display-with-window-symbolic");
    renderTBtn.set_tooltip_text("Enable renderer");
    mainBox.append(renderTBtn);

    debugOverlayTBtn.property_active().signal_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_debug_overlay_toggled));
    debugOverlayTBtn.set_icon_name("closed-captioning-symbolic");

    // Glib::RefPtr<Gtk::IconPaintable> icon = Gtk::IconTheme::get_for_display(Gdk::Display::get_default())->lookup_icon("list-symbolic", 64);
    // Gtk::Image* iconImage = Gtk::make_managed<Gtk::Image>();
    // iconImage->set_from_icon_name(icon->get_icon_name());
    // debugOverlayTBtn.set_child(*iconImage);
    // debugOverlayTBtn.set_icon_name(icon->get_icon_name());

    debugOverlayTBtn.set_active();
    debugOverlayTBtn.set_tooltip_text("Toggle debug overlay");
    mainBox.append(debugOverlayTBtn);

    speciesPreviewOverlayTBtn.property_active().signal_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_species_preview_overlay_toggled));
    speciesPreviewOverlayTBtn.set_icon_name("background-app-ghost-symbolic");

    speciesPreviewOverlayTBtn.set_active();
    speciesPreviewOverlayTBtn.set_tooltip_text("Toggle species preview overlay");
    mainBox.append(speciesPreviewOverlayTBtn);

    colorChangeTBtn.property_active().signal_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_color_change_toggled));
    colorChangeTBtn.set_icon_name("color-symbolic");

    colorChangeTBtn.set_active(false);
    colorChangeTBtn.set_tooltip_text("Toggle automatic color change");
    mainBox.append(colorChangeTBtn);

    zoomInBtn.signal_clicked().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_zoom_in_clicked));
    zoomInBtn.set_tooltip_text("Zoom in");
    zoomInBtn.set_icon_name("zoom-in");
    zoomInBtn.set_sensitive(true);
    zoomBox.append(zoomInBtn);

    zoomOutBtn.signal_clicked().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_zoom_out_clicked));
    zoomOutBtn.set_tooltip_text("Zoom out");
    zoomOutBtn.set_icon_name("zoom-out");
    zoomBox.append(zoomOutBtn);

    zoomFitBtn.signal_clicked().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_zoom_fit_clicked));
    zoomFitBtn.set_tooltip_text("Zoom fit");
    zoomFitBtn.set_icon_name("zoom-fit-best");
    zoomBox.append(zoomFitBtn);

    zoomResetBtn.signal_clicked().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_zoom_reset_clicked));
    zoomResetBtn.set_tooltip_text("Zoom reset");
    zoomResetBtn.set_icon_name("zoom-original");
    zoomBox.append(zoomResetBtn);

    blurTBtn.property_active().signal_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_blur_toggled));
    blurTBtn.set_icon_name("blur-symbolic");
    blurTBtn.set_tooltip_text("Blur entities");
    blurTBtn.set_active(simulation->is_blur_enabled());
    miscBox.append(blurTBtn);

    std::vector<Glib::ustring> speciesNames;
    for (size_t i = 0; i < simulation->get_species()->size(); i++) {
        speciesNames.emplace_back(std::to_string(i));
    }
    speciesDropDown.set_model(Gtk::StringList::create(speciesNames));
    speciesDropDown.set_selected(0);
    speciesDropDown.set_tooltip_text("Change the selected species");
    speciesDropDown.property_selected().signal_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_species_selection_changed));
    speciesBox.append(speciesDropDown);

    speciesColorBtn.signal_color_set().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_species_color_set));
    speciesColorBtn.set_tooltip_text("Species color");
    speciesBox.append(speciesColorBtn);

    speciesTurnSpeedBtn.set_tooltip_text("Species turning speed");
    speciesTurnSpeedBtn.set_icons({"rotation-allowed-symbolic"});
    speciesTurnSpeedBtn.signal_value_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_species_turn_speed_changed));
    speciesBox.append(speciesTurnSpeedBtn);

    speciesSensorAngleDegreesBtn.set_tooltip_text("Species sensor angle");
    speciesSensorAngleDegreesBtn.set_icons({"object-straighten-symbolic"});
    speciesSensorAngleDegreesBtn.signal_value_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_species_sensor_angle_changed));
    speciesBox.append(speciesSensorAngleDegreesBtn);

    speciesSensorSizeBtn.set_tooltip_text("Species sensor size");
    speciesSensorSizeBtn.set_icons({"image-resize-symbolic"});
    speciesSensorSizeBtn.signal_value_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_species_sensor_size_changed));
    speciesBox.append(speciesSensorSizeBtn);

    speciesSensorOffsetBtn.set_tooltip_text("Species sensor offset");
    speciesSensorOffsetBtn.set_icons({"hig-symbolic"});
    speciesSensorOffsetBtn.signal_value_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_species_sensor_offset_changed));
    speciesBox.append(speciesSensorOffsetBtn);

    speciesAddSlimesTBtn.property_active().signal_changed().connect(sigc::mem_fun(*this, &SimulationSettingsBarWidget::on_species_add_slimes_toggled));
    speciesAddSlimesTBtn.set_icon_name("add-symbolic");
    speciesAddSlimesTBtn.set_tooltip_text("Add slimes to species");
    speciesAddSlimesTBtn.set_active(false);
    // speciesBox.append(speciesAddSlimesTBtn);

    // Trigger species selection update:
    on_species_selection_changed();
}

//-----------------------------Events:-----------------------------
void SimulationSettingsBarWidget::on_simulate_toggled() {
    assert(simulation);
    simulation->set_running(simulateTBtn.get_active());
    if (simulateTBtn.get_active()) {
        simulateTBtn.set_icon_name("pause-large-symbolic");
    } else {
        simulateTBtn.set_icon_name("play-large-symbolic");
    }
}

void SimulationSettingsBarWidget::on_render_toggled() {
    assert(simWidget);
    simWidget->enableUiUpdates = renderTBtn.get_active();
}

void SimulationSettingsBarWidget::on_debug_overlay_toggled() {
    assert(simOverlayWidget);
    simOverlayWidget->set_debug_overlay_enabled(debugOverlayTBtn.get_active());
}

void SimulationSettingsBarWidget::on_species_preview_overlay_toggled() {
    assert(simOverlayWidget);
    simOverlayWidget->set_species_preview_overlay_enabled(speciesPreviewOverlayTBtn.get_active());
}

void SimulationSettingsBarWidget::on_color_change_toggled() {
    assert(simOverlayWidget);
    simulation->set_color_change_enabled(colorChangeTBtn.get_active());
}

void SimulationSettingsBarWidget::on_zoom_in_clicked() {
    assert(simWidget);
    float zoomFactor = simWidget->get_zoom_factor();
    zoomFactor *= 1.25;
    if (zoomFactor >= 2) {
        zoomFactor = 2;
        zoomInBtn.set_sensitive(false);
    }
    simWidget->set_zoom_factor(zoomFactor);
}

void SimulationSettingsBarWidget::on_zoom_out_clicked() {
    assert(simWidget);
    float zoomFactor = simWidget->get_zoom_factor();
    zoomFactor *= 0.75;
    zoomInBtn.set_sensitive(true);
    simWidget->set_zoom_factor(zoomFactor);
}

void SimulationSettingsBarWidget::on_zoom_reset_clicked() {
    assert(simWidget);
    simWidget->set_zoom_factor(1.0);
}

void SimulationSettingsBarWidget::on_zoom_fit_clicked() {
    assert(simWidget);
    assert(simulation);
    float zoomX = static_cast<float>(simWidget->get_width()) / static_cast<float>(simulation->get_width());
    float zoomY = static_cast<float>(simWidget->get_height()) / static_cast<float>(simulation->get_height());
    float zoomFactor = zoomX > zoomY ? zoomY : zoomX;
    simWidget->set_zoom_factor(zoomFactor);
    zoomInBtn.set_sensitive(true);
}

void SimulationSettingsBarWidget::on_blur_toggled() {
    assert(simulation);
    simulation->set_blur_enabled(blurTBtn.get_active());
}

void SimulationSettingsBarWidget::on_species_selection_changed() {
    guint index = speciesDropDown.get_selected();

    sim::Rgba color = (*simulation->get_species())[index].color;
    speciesColorBtn.property_rgba().set_value(Gdk::RGBA(color.r, color.g, color.b, color.a));

    speciesTurnSpeedBtn.set_value(static_cast<double>((*simulation->get_species())[index].turnSpeed));

    speciesSensorAngleDegreesBtn.set_value(static_cast<double>((*simulation->get_species())[index].sensorAngleDegrees));

    speciesSensorSizeBtn.set_value(static_cast<double>((*simulation->get_species())[index].sensorSize));

    speciesSensorOffsetBtn.set_value(static_cast<double>((*simulation->get_species())[index].sensorOffsetDst));

    assert(simWidget);
    simWidget->set_species_index(static_cast<uint32_t>(index));
    simOverlayWidget->set_species_index(static_cast<uint32_t>(index));
}

void SimulationSettingsBarWidget::on_species_color_set() {
    assert(simulation);
    guint index = speciesDropDown.get_selected();
    Gdk::RGBA color = speciesColorBtn.property_rgba();
    (*simulation->get_species())[index].color = sim::Rgba{color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()};
    simulation->set_species_need_sync(true);
}

void SimulationSettingsBarWidget::on_species_turn_speed_changed(double value) {
    assert(simulation);
    guint index = speciesDropDown.get_selected();
    (*simulation->get_species())[index].turnSpeed = static_cast<float>(value);
    simulation->set_species_need_sync(true);
}

void SimulationSettingsBarWidget::on_species_sensor_angle_changed(double value) {
    assert(simulation);
    guint index = speciesDropDown.get_selected();
    (*simulation->get_species())[index].sensorAngleDegrees = static_cast<float>(value);
    simulation->set_species_need_sync(true);
}

void SimulationSettingsBarWidget::on_species_sensor_size_changed(double value) {
    assert(simulation);
    guint index = speciesDropDown.get_selected();
    (*simulation->get_species())[index].sensorSize = static_cast<int>(value);
    simulation->set_species_need_sync(true);
}

void SimulationSettingsBarWidget::on_species_sensor_offset_changed(double value) {
    assert(simulation);
    guint index = speciesDropDown.get_selected();
    (*simulation->get_species())[index].sensorOffsetDst = static_cast<float>(value);
    simulation->set_species_need_sync(true);
}

void SimulationSettingsBarWidget::on_species_add_slimes_toggled() {
    assert(simWidget);
    simWidget->set_species_add_enabled(speciesAddSlimesTBtn.get_active());
}

}  // namespace ui::widgets
