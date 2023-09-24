cmake_minimum_required(VERSION 3.4)
project(spetrum-analyzer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS
    Widgets
    REQUIRED)

add_library(${PROJECT_NAME} SHARED
    src/spectrum-analyzer.h
    src/spectrum-analyzer-ctrl.h
    src/plugin-factory.h
    src/spectrum-process.h
    src/spectrum-common.h
    src/plot-menu.h
    src/spectrum-plot.h
    src/plot-peaks.h
    src/abstract-text-label.h
    src/settings-text-label.h
    src/peaks-text-label.h
    src/export.cpp
    src/spectrum-analyzer.cpp
    src/spectrum-analyzer-ctrl.cpp
    src/spectrum-process.cpp
    src/plot-menu.cpp
    src/spectrum-plot.cpp
    src/plot-peaks.cpp
    src/abstract-text-label.cpp
    src/settings-text-label.cpp
    src/peaks-text-label.cpp
)

add_subdirectory(qwt EXCLUDE_FROM_ALL)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    qwt
    dsplib
    dsplab-sdk)
