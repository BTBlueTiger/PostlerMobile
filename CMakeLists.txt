cmake_minimum_required(VERSION 3.16)
project(Postler_Mobile LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Location
    Qml
    Quick
    Core5Compat
    Network
    Positioning
    Sensors
    Sql
    SerialPort

)


qt_standard_project_setup(REQUIRES 6.6)

qt_add_executable(Postler_Mobile
    main.cpp qrc.qrc
)

qt_add_qml_module(Postler_Mobile
    URI routeguide

    SOURCES


    RoutingManager.h RoutingManager.cpp
    PostlerStorage.h PostlerStorage.cpp
    RequestReceiver.h

    Constants.h

    RESOURCES
        QML_FILES main.qml
        SOURCES

        include/PostlerStorage/Database.h

        include/PostlerStorage/PostlerStorage.h
        src/PostlerStorage/PostlerStorage.cpp

        include/PostlerStorage/Model/Table.h
        src/PostlerStorage/Model/Table.cpp

        include/PostlerStorage/TableValue.h

        include/PostlerStorage/Model/RouteTable.h
        src/PostlerStorage/Model/RouteTable.cpp

        include/PostlerStorage/Model/ZabTable.h
        src/PostlerStorage/Model/ZabTable.cpp

        include/PostlerStorage/Model/HouseNumberTable.h
        src/PostlerStorage/Model/HouseNumberTable.cpp

        src/PostlerStorage/Database.cpp
        include/PostlerStorage/Database.h

        include/PostlerStorage/Model/StopTable.h
        src/PostlerStorage/Model/StopTable.cpp
        SOURCES
        RESOURCES qml/custom/qmldir
)




set_target_properties(Postler_Mobile PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

if (ANDROID)
    include(FetchContent)
    FetchContent_Declare(
        android_openssl
        DOWNLOAD_EXTRACT_TIMESTAMP true
        URL https://github.com/KDAB/android_openssl/archive/refs/heads/master.zip
    )
    FetchContent_MakeAvailable(android_openssl)
    include(${android_openssl_SOURCE_DIR}/android_openssl.cmake)
endif()

target_link_libraries(Postler_Mobile PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Location
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
    Qt6::Core5Compat
    Qt6::Sql
    Qt6::SerialPort
    Qt6::Sensors
)

add_subdirectory(modules)

target_link_libraries(Postler_Mobile PRIVATE
    ScreenResolutionplugin
    OSMDatabaseplugin
    NominatimRessourceplugin
    GeoPositionRessourceplugin
)


#qt_add_translations(${EXECUTABLE_NAME} TS_FILES
#        ${EXECUTABLE_NAME}_de_DE.ts
#        ${EXECUTABLE_NAME}_en_EN.ts
#    SOURCES ${QML_SOURCES2} ${CPP_SOURCES} ${HPP_SOURCES}
#    RESOURCE_PREFIX "/translations"
#)


install(TARGETS ${EXECUTABLE_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)


