cmake_minimum_required(VERSION 3.4)
project(dsplab-sdk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    dsplab/controller.cpp
    dsplab/device.cpp
    dsplab/display-value.cpp
    dsplab/frame.cpp
    dsplab/port-input.cpp
    dsplab/port-output.cpp
    dsplab/timer/timer.cpp
    dsplab/i_device.h)

find_package(
  Qt5
  COMPONENTS Core
  REQUIRED)

add_library(dsplab-sdk STATIC ${SOURCES})
target_link_libraries(dsplab-sdk Qt5::Core)
target_include_directories(dsplab-sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
