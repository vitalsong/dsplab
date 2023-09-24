cmake_minimum_required(VERSION 3.4)
project(sinad-meter LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

CPMAddPackage(NAME dsplib
    GIT_REPOSITORY "git@github.com:vitalsong/dsplib.git"
    VERSION 0.41.0
    EXCLUDE_FROM_ALL ON
)

add_library(${PROJECT_NAME} SHARED
    src/export.cpp
    src/sinad-ctrl.cpp
    src/sinad-dev.cpp
    src/sinad-meter.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets dsplab-sdk dsplib)
