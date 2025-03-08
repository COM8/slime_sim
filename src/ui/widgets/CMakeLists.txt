cmake_minimum_required(VERSION 3.16)

add_library(ui_widgets SimulationWidget.cpp
    SimulationWidget.hpp
    SimulationOverlayWidget.cpp
    SimulationOverlayWidget.hpp
    SimulationSettingsBarWidget.cpp
    SimulationSettingsBarWidget.hpp
    SpeciesPreviewWidget.cpp
    SpeciesPreviewWidget.hpp
    SpeciesPreviewCairoWidget.cpp
    SpeciesPreviewCairoWidget.hpp
    TitleWidget.cpp
    TitleWidget.hpp)

target_link_libraries(ui_widgets PRIVATE PkgConfig::GTKMM logger PkgConfig::EPOXY ui_widgets_opengl ui_widgets_opengl_fb sim)

add_subdirectory(opengl)
