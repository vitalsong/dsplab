cmake_minimum_required(VERSION 3.10)
project(dsplab LANGUAGES C CXX VERSION 0.1.0)

set(DSPLAB_PLUGINS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/plugins")
set(DSPLAB_APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(cmake/CPM.cmake)

add_subdirectory(dsplab-sdk)
add_subdirectory(dsplab-gui)
add_subdirectory(plugins)
