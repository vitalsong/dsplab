cmake_minimum_required(VERSION 3.4)
project(upd-receiver LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Network Widgets REQUIRED)

add_library(${PROJECT_NAME} SHARED
		src/export.cpp
		src/udp-receiver-ctrl.cpp
		src/udp-receiver-dev.cpp
        src/main-window.ui)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} Qt5::Network Qt5::Widgets dsplab-sdk)
