#include "OSMDatabase.h"
#include "iostream"

#include "../libosmium/include/osmium/io/file.hpp"
#include "../libosmium/include/osmium/io/reader.hpp"
#include "../libosmium/include/osmium/osm/entity_bits.hpp"
#include "../libosmium/include/osmium/memory/buffer.hpp"
#include "../libosmium/include/osmium/visitor.hpp"


OSMDatabase::OSMDatabase(const QString &dataBasePath)
{
    osmDatabase = QSqlDatabase::addDatabase("QSQLITE");

    QFileInfo dbFileInfo(dataBasePath);

    osmDatabase.setDatabaseName(dataBasePath);
    qDebug() << dataBasePath;

    if (dbFileInfo.exists() && dbFileInfo.isFile()) {
        qint64 size = dbFileInfo.size(); // Get the file size in bytes
        qDebug() << "Database size:" << size << "bytes";
        if(size >   1700000)
        {
            qDebug() << "Database exists";
            dataBaseAvailable = true;
        }else {
            dataBaseAvailable = false;
        }
    } else {
        dataBaseAvailable = false;
        qDebug() << "Database does not exist at:" << dataBasePath;
    }

    if (!osmDatabase.open())
    {
        qDebug() << "Error: connection with Local Database fail";
    }
    else
    {
        qDebug() << "Local Database: connection ok";
    }


    if(!dataBaseAvailable)
        handler = new PBFFileHandler(
            &m_townModel,
            &m_streetModel,
            &m_houseNumberModel
            );

}

OSMDatabase::PBFFileHandler::PBFFileHandler(
    OSMDataBaseModel::TownModel* townModel,
    OSMDataBaseModel::StreetModel* streetModel,
    OSMDataBaseModel::HouseNumberModel* houseNumberModel)
    : m_townModel(townModel), m_streetModel(streetModel), m_houseNumberModel(houseNumberModel)
{
}

OSMDatabase::~OSMDatabase()
{

    if(handler)
    {
        delete handler;
        handler = nullptr;
    }

}


void OSMDatabase::createDatabase(const QString &inputPath)
{

    if(dataBaseAvailable) return;

    qDebug() << "Creating Database";

    QFileInfo info(inputPath);
    osmium::io::File input_file{info.absoluteFilePath().toStdString()};
    osmium::io::Reader reader{input_file, osmium::osm_entity_bits::node};

    m_streetModel.createTable();
    m_houseNumberModel.createTable();
    m_townModel.createTable();

    // Begin a transaction for efficiency
    osmDatabase.transaction();

    while (osmium::memory::Buffer buffer = reader.read())
        osmium::apply(buffer, *handler);  // Process the buffer with your custom handler


    qDebug() << "Apply finish";
    // Commit the transaction
    if (!osmDatabase.commit()) {
        std::cout << "Error committing transaction:";
    }

    qDebug() << "Database created";

}


void OSMDatabase::PBFFileHandler::node(const osmium::Node& node)
{
    /*
    if(node.tags()["addr:housenumber"])
    {
        QString number = node.tags()["addr:housenumber"];
        QString street = node.tags()["addr:street"];
        QString city = node.tags()["addr:city"];
        QString postcode = node.tags()["addr:postcode"];

        osmium::Location location = node.location();

        m_townModel->insert({{"city", city}, {"postcode", postcode}});
        m_streetModel->insert({{"street", street}, {"postcode", postcode}});
        m_houseNumberModel->insert({{"street", street}, {"number", number}, {"lat", location.lat() }, {"lon", location.lon()}});
    }
*/

}

void OSMDatabase::PBFFileHandler::way(const osmium::Way& way) {
    const char* house_number = way.tags()["addr:housenumber"];
    const char* street = way.tags()["addr:street"];
    const char* city = way.tags()["addr:city"];
    const char* postcode = way.tags()["addr:postcode"];

    qDebug() << house_number;
}


