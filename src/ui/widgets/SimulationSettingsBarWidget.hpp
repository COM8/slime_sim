#pragma once

#include "SimulationOverlayWidget.hpp"
#include "SimulationWidget.hpp"
#include "sim/Simulation.hpp"
#include "ui/widgets/SimulationOverlayWidget.hpp"
#include <memory>
#include <gtkmm/box.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/scalebutton.h>
#include <gtkmm/switch.h>
#include <gtkmm/togglebutton.h>

namespace ui::widgets {
class SimulationSettingsBarWidget : public Gtk::Box {
 private:
    std::shared_ptr<sim::Simulation> simulation{sim::Simulation::get_instance()};

    SimulationWidget* simWidget{nullptr};
    SimulationOverlayWidget* simOverlayWidget{nullptr};

    Gtk::Box mainBox;
    Gtk::Box zoomBox;
    Gtk::Box miscBox;
    Gtk::Box speciesBox;

    Gtk::ToggleButton simulateTBtn;
    Gtk::ToggleButton renderTBtn;
    Gtk::ToggleButton debugOverlayTBtn;

    Gtk::Button zoomInBtn;
    Gtk::Button zoomOutBtn;
    Gtk::Button zoomResetBtn;
    Gtk::Button zoomFitBtn;

    Gtk::ToggleButton blurTBtn;

    Gtk::DropDown speciesDropDown;
    Gtk::ColorButton speciesColorBtn;
    Gtk::ScaleButton speciesTurnSpeedBtn;
    Gtk::ScaleButton speciesSensorAngleDegreesBtn;
    Gtk::ScaleButton speciesSensorSizeBtn;
    Gtk::ScaleButton speciesSensorOffsetBtn;

 public:
    SimulationSettingsBarWidget(SimulationWidget* simWidget, SimulationOverlayWidget* simOverlayWidget);

 private:
    void prep_widget();

    //-----------------------------Events:-----------------------------
    void on_simulate_toggled();
    void on_render_toggled();
    void on_debug_overlay_toggled();
    void on_zoom_in_clicked();
    void on_zoom_out_clicked();
    void on_zoom_reset_clicked();
    void on_zoom_fit_clicked();
    void on_blur_toggled();
    void on_species_color_set();
    void on_species_turn_speed_changed(double value);
    void on_species_sensor_angle_changed(double value);
    void on_species_sensor_size_changed(double value);
    void on_species_sensor_offset_changed(double value);
    void on_species_selection_changed();
};
}  // namespace ui::widgets
