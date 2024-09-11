// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QGuiApplication>
#include <QQmlApplicationEngine>


//#include "app_environment.h"
//#include "import_qml_components_plugins.h"
//#include "import_qml_plugins.h"


#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include "QProcess"
#include "QDesktopServices"

#include "modules/Utility/ScreenResolution/ScreenResolution.h"
#include "modules/NominatimRessource/NominatimRessource.h"
#include "modules/GeoPositionRessource/GeoPositionRessource.h"

#include "include/PostlerStorage/Database.h"

#include "RoutingManager.h"


//Q_IMPORT_QML_PLUGIN(ScreenResolutionPlugin)
//Q_IMPORT_QML_PLUGIN(ValidationTextfieldPlugin)

void registerModules()
{
    qmlRegisterType<ScreenResolution>("ScreenResolution", 1, 0, "ScreenResolution");

    /*
    qmlRegisterType<OSMDataBaseModel::TownModel>("modules.OSMDatabase.OSMDataBaseModel.TownModel", 0, 1, "TownModel");
    qmlRegisterType<OSMDataBaseModel::StreetModel>("StreetModel", 0, 1, "StreetModel");
    qmlRegisterType<OSMDataBaseModel::HouseNumberModel>("HouseNumberModel", 0, 1, "HouseNumberModel");

    qmlRegisterType<PostlerStop::HouseNumberListModel>("HouseNumberListModel", 0, 1, "HouseNumberListModel");
    qmlRegisterType<PostlerStop::StreetListModel>("StreetListModel", 0, 1, "StreetListModel");
    qmlRegisterType<CSVToStops>("CSVToStops", 1, 0, "CSVToStops");
qmlRegisterType<GeoPositionRessource>("GeoPositionRessource", 1, 0, "GeoPositionRessource");
*/

    qRegisterMetaType<PostlerStorage::Database::Filter_T>("PostlerStorage::Filter_t");

    qmlRegisterType<PostlerStorage::ZabTable>("ZabTable", 1, 0, "ZabTable");
    qRegisterMetaType<PostlerStorage::ZabTable::TABLE_ROLES>("ZabTable::TableRoles");

    qmlRegisterType<PostlerStorage::StopTable>("StopTable", 1, 0, "StopTable");
    qRegisterMetaType<PostlerStorage::ZabTable::TABLE_ROLES>("StopTable::TableRoles");
}

void registerSingletonModules()
{
    qmlRegisterSingletonType<NominatimRessource>("NominatimRessource", 0, 1, "NominatimRessource", NominatimRessource::createSingletonInstance);
    qmlRegisterSingletonType<GeoPositionRessource>("GeoPositionRessource", 1, 0, "GeoPositionRessource", GeoPositionRessource::createSingletonInstance);

    /*
    qmlRegisterSingletonType<PostlerStorage::DataBase>("PostlerDataBase", 0, 1, "PostlerDataBase", PostlerStorage::DataBase::createSingletonInstance);
    qmlRegisterSingletonType<PostlerStorage::RoutingManagerTableModel>("RoutingManagerTableModel", 0, 1, "RoutingManagerTableModel", PostlerStorage::RoutingManagerTableModel::createSingletonInstance);
    qmlRegisterSingletonType<PostlerStorage::ZabTableModel>("ZabTableModel", 0, 1, "ZabTableModel", PostlerStorage::ZabTableModel::createSingletonInstance);
    qmlRegisterSingletonType<PostlerStorage::HouseNumberTableModel>("HouseNumberTableModel", 0, 1, "HouseNumberTableModel", PostlerStorage::HouseNumberTableModel::createSingletonInstance);
    */
    qmlRegisterSingletonType<PostlerStorage::Database>("PostlerStorage", 0, 1, "PostlerStorage", PostlerStorage::Database::createSingletonInstance);

}

void registerUncreateAbleType()
{
    qmlRegisterUncreatableType<HousNumber::ZabStopStates>("ZabStopStates", 1, 0, "ZabStopStates", "Not creatable as it is an enum type");
}


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    registerModules();
    registerSingletonModules();
    registerUncreateAbleType();

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
