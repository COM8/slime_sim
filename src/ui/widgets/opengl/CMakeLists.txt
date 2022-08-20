cmake_minimum_required(VERSION 3.16)

add_library(ui_widgets_opengl AbstractGlObject.hpp
                              AbstractGlObject.cpp
                              SlimeGlObject.hpp
                              SlimeGlObject.cpp
                              BlurGlObject.hpp
                              BlurGlObject.cpp
                              Slime.cpp
                              Slime.hpp
                              ScreenSquareGlObject.cpp
                              ScreenSquareGlObject.hpp)

target_link_libraries(ui_widgets_opengl PRIVATE logger PkgConfig::EPOXY PkgConfig::GTKMM ui_widgets_opengl_utils)

add_subdirectory(utils)
add_subdirectory(fb)
