cmake_minimum_required(VERSION 3.1.0)
project(qwt LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# настройки конфигурации
option(QwtPlot ON)
option(QwtSvg OFF)
option(QwtOpenGL OFF)
option(QwtWidgets OFF)

add_definitions(-DQWT_MOC_INCLUDE)

if(NOT QwtSvg)
    add_definitions(-DQWT_NO_SVG)
endif()

if(NOT QwtOpenGL)
    add_definitions(-DQWT_NO_OPENGL)
endif()

# каталог с исходниками
FILE(GLOB_RECURSE QWT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

find_package(Qt5 COMPONENTS Widgets Concurrent PrintSupport REQUIRED)

add_library(qwt STATIC ${QWT_SOURCES})

target_link_libraries(qwt
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Concurrent
)

target_include_directories(qwt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
