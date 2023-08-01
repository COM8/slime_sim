#include "MainWindow.hpp"
#include "sim/Simulation.hpp"
#include <cassert>
#include <gtkmm/window.h>

namespace ui::windows {
MainWindow::MainWindow() : simulationOverlayWidget(&simulationWidget),
                           simulationSettingsBarWidget(&simulationWidget, &simulationOverlayWidget) { prep_window(); }

void MainWindow::prep_window() {
    set_title("Slime Simulator");
    set_default_size(800, 550);

#if DEBUG
    add_css_class("devel");
#endif

    // Keyboard events:
    Glib::RefPtr<Gtk::EventControllerKey> controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(sigc::mem_fun(*this, &MainWindow::on_key_pressed), false);
    add_controller(controller);

    // Header bar:
    Gtk::HeaderBar* headerBar = Gtk::make_managed<Gtk::HeaderBar>();
    inspectorBtn.set_label("🐞");
    inspectorBtn.set_tooltip_text("Inspector");
    inspectorBtn.signal_clicked().connect(&MainWindow::on_inspector_btn_clicked);
    headerBar->pack_end(inspectorBtn);
    set_titlebar(*headerBar);

    // Settings:
    mainBox.append(simulationSettingsBarWidget);

    // Simulator:
    simulationWidget.set_expand();
    simulationOverlay.set_child(simulationWidget);
    simulationOverlay.add_overlay(simulationOverlayWidget);
    simulationOverlay.set_expand();

    titleOverlay.set_child(simulationOverlay);
    titleOverlay.add_overlay(titleWidget);
    titleOverlay.set_expand();
    titleWidget.set_expand();
    titleWidget.set_visible(false);
    mainBox.append(titleOverlay);

    set_child(mainBox);
}

//-----------------------------Events:-----------------------------
void MainWindow::on_inspector_btn_clicked() {
    gtk_window_set_interactive_debugging(true);
}

bool MainWindow::on_key_pressed(guint keyVal, guint /*keyCode*/, Gdk::ModifierType /*modifier*/) {
    if (keyVal == GDK_KEY_F11) {
        if (is_fullscreen()) {
            unfullscreen();
        } else {
            fullscreen();
        }
        return true;
    }
    if (keyVal == GDK_KEY_Escape && is_fullscreen()) {
        unfullscreen();
        return true;
    }

    // Toggle menu bar
    if ((keyVal == GDK_KEY_M) || (keyVal == GDK_KEY_m)) {
        simulationSettingsBarWidget.set_visible(!simulationSettingsBarWidget.get_visible());
    }

    // Toggle simulation
    if ((keyVal == GDK_KEY_P) || (keyVal == GDK_KEY_p)) {
        sim::Simulation::get_instance()->set_running(!sim::Simulation::get_instance()->is_running());
    }

    // Toggle simulation
    if ((keyVal == GDK_KEY_T) || (keyVal == GDK_KEY_t)) {
        titleWidget.set_visible(!titleWidget.get_visible());
    }
    return false;
}
}  // namespace ui::windows
