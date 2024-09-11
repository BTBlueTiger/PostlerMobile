cmake_minimum_required(VERSION 3.16)

project(NominatimRessource VERSION 0.1 LANGUAGES CXX)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


include_directories(${OSMIUM_INCLUDE_DIRS})


qt_add_library(NominatimRessource STATIC)

qt_add_qml_module(NominatimRessource
    URI
        NominatimRessource
    SOURCES
        NominatimRessource.h NominatimRessource.cpp
)

target_link_libraries(OSMDatabase PRIVATE
    Qt6::Core
    Qt6::Qml
    Qt6::Quick
    Qt6::Sql
)
