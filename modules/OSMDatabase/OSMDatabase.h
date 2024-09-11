#ifndef OSMDATABASE_H
#define OSMDATABASE_H


#include "OsmDataBaseModel.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

#include <QFileInfo>

#include "../libosmium/include/osmium/handler.hpp"

/*
#include <osmium/osm/location.hpp>
#include <osmium/io/file.hpp>
#include <osmium/io/reader.hpp>
#include <osmium/io/pbf_input.hpp>
#include <osmium/io/pbf_output.hpp>
*/
class OSMDatabase
{
public:
    OSMDatabase(const QString &dataBasePath);
    ~OSMDatabase();

    void createDatabase(const QString &inputPath);

private:

    class PBFFileHandler : public osmium::handler::Handler
    {

        public:
        PBFFileHandler(
            OSMDataBaseModel::TownModel* townModel,
            OSMDataBaseModel::StreetModel* streetModel,
            OSMDataBaseModel::HouseNumberModel* houseNumberModel
            );
            void node(const osmium::Node& node);
            void way(const osmium::Way& way);

        private:
        OSMDataBaseModel::TownModel* m_townModel;
        OSMDataBaseModel::StreetModel* m_streetModel;
        OSMDataBaseModel::HouseNumberModel* m_houseNumberModel;
    };




    PBFFileHandler* handler = nullptr;

    QSqlDatabase osmDatabase;
    QSqlQuery query;

    OSMDataBaseModel::TownModel m_townModel;
    OSMDataBaseModel::StreetModel m_streetModel;
    OSMDataBaseModel::HouseNumberModel m_houseNumberModel;


    const char* m_city;
    const char* m_postcode;

    bool dataBaseAvailable;

};

#endif // OSMDATABASE_H
