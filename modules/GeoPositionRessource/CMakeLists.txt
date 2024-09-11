cmake_minimum_required(VERSION 3.16)

project(GeoPositionRessource VERSION 0.1 LANGUAGES CXX)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


include_directories(${OSMIUM_INCLUDE_DIRS})


qt_add_library(GeoPositionRessource STATIC)

qt_add_qml_module(GeoPositionRessource
    URI
        GeoPositionRessource
    SOURCES
        GeoPositionRessource.h GeoPositionRessource.cpp
)

target_link_libraries(GeoPositionRessource PRIVATE
    Qt6::Core
    Qt6::Qml
    Qt6::Quick
    Qt6::Location
    Qt6::SerialPort
)
