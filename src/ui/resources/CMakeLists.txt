cmake_minimum_required(VERSION 3.20)

set(RESOURCE_DEF ${CMAKE_CURRENT_SOURCE_DIR}/ui_resources.c.xml PARENT_SCOPE)
set(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set(RESOURCE_DEPS ${CMAKE_CURRENT_SOURCE_DIR}/theme.css

    ${CMAKE_CURRENT_SOURCE_DIR}/shader/slime/slime.comp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/screen_square/screen_square.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/screen_square/screen_square.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/screen_square/screen_square.geom
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/blur/blur.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/blur/blur.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/blur/blur.geom
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/species_preview/species_preview.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/species_preview/species_preview.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shader/species_preview/species_preview.geom

    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/closed-captioning-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/display-with-window-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/pause-large-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/pencil-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/play-large-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/test-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/blur-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/hig-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/image-resize-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/object-straighten-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/rotation-allowed-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/plus-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/transparent-background-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/background-app-ghost-symbolic.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/scalable/action/color-symbolic.svg PARENT_SCOPE)
