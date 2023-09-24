cmake_minimum_required(VERSION 3.4)
project(hilbert LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

CPMAddPackage(NAME dsplib
    GIT_REPOSITORY "git@github.com:vitalsong/dsplib.git"
    VERSION 0.41.0
    EXCLUDE_FROM_ALL ON
)

add_library(${PROJECT_NAME} SHARED
        src/hilbert-transform.cpp
        src/hilbert-transform-ctrl.cpp
        src/export.cpp)

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        dsplab-sdk
        dsplib)
