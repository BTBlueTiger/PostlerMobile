cmake_minimum_required(VERSION 3.16)

project(ScreenResolution VERSION 0.1 LANGUAGES CXX)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

qt_add_library(ScreenResolution STATIC)

qt_add_qml_module(ScreenResolution
    URI
        ScreenResolution
    SOURCES
        ScreenResolution.h ScreenResolution.cpp
)
