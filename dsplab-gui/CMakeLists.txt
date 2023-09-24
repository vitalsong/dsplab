cmake_minimum_required(VERSION 3.4)
project(dsplab-gui LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DSPLAB_APP_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DSPLAB_APP_PATH})

set(SOURCES
    src/device-config.cpp
    src/main.cpp
    src/main-window.cpp
    src/main-window.ui
    src/base/device-facade.cpp
    src/base/port-adapter.cpp
    src/loading/plugin-find-widget.cpp
    src/loading/plugin-loader.cpp
    src/loading/plugin-loader-widget.cpp
    src/loading/plugin-tree-widget.cpp
    src/loading/plugin-loader.ui
    src/logging/logger.cpp
    src/menu/menu-bar.cpp
    src/scene/dragdrop-device.cpp
    src/scene/graphics-connection.cpp
    src/scene/graphics-device.cpp
    src/scene/graphics-port.cpp
    src/scene/main-scene.cpp
    src/scene/main-view.cpp
    src/scene/scene-tool-tip.cpp)

if (WIN32)
    set(SOURCES ${SOURCES}
        src/sys-monitor/cpu-monitor.cpp
        src/sys-monitor/cpu-monitor-win.cpp
        src/sys-monitor/qcpu-monitor.cpp)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    dsplab-sdk
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
