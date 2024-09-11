#include "PostlerStorage.h"
#include "Constants.h"

using namespace PostlerStorage;

HouseNumberTableModel *HouseNumberTableModel::m_instance = nullptr;
ZabTableModel *ZabTableModel::m_instance = nullptr;
RoutingManagerTableModel *RoutingManagerTableModel::m_instance = nullptr;
DataBase *DataBase::m_instance = nullptr;


QString TableModel::tableName() const
{
    return m_tableName;
}

TableModel::TableModel(QObject* parent) : QSqlTableModel(parent)
{

}

bool TableModel::tableNotExists()
{
    return QSqlDatabase::database().tables().contains(tableName());
}




DataBase::DataBase()
{

}

bool DataBase::open()
{
    m_postlerDatabase = QSqlDatabase::addDatabase("QSQLITE");
    m_postlerDatabase.setDatabaseName(POSTLER_STORAGE_PATH);
    if (!m_postlerDatabase.open()) {
        qDebug() << "Error: Failed to open or create the database: " << m_postlerDatabase.lastError().text();
        return false;
    } else {
        qDebug() << "Database opened or created successfully!";
        return true;
    }
}



bool DataBase::toDatabase(const RoutingManager* routingManger, const QString& name) const
{

    QVariantMap map;
    map["name"] = name;

    int rm_id = RoutingManagerTableModel::m_instance->insert(
        QVariantMap({ {"name", QVariant::fromValue(name)} })
        );
    qDebug() << "rm_id" << rm_id;
    if(rm_id >= 0)
    {
        QVector<ZabModel*> zabs = routingManger->zabs();
        for(int i = 0; i < zabs.size(); i++)
        {
            ZabModel* zm = zabs[i];
            int zm_id = ZabTableModel::m_instance->insertOrUpdate(
                QVariantMap({
                    {{"_routing_manager"}, rm_id},
                    {{"position"}, i}
                }),
                QVariantMap({
                    {"_routing_manager", QVariant::fromValue(rm_id)},
                    {"position", QVariant::fromValue(i)},
                    {"postal_code", zm->postalCode()},
                    {"street", zm->street()}
                })
                );
            qDebug() << zm_id;
            if(zm_id > 0)
            {
                QVector<HousNumber *> houseNumbers = zm->houseNumbers();

                for(int j = 0; j < houseNumbers.size(); j++)
                {
                    HousNumber* hm = houseNumbers[j];


                    int hm_id = HouseNumberTableModel::m_instance->insertOrUpdate(
                        QVariantMap({
                            {{"_zab"}, QVariant::fromValue(zm_id)},
                            {{"position"}, QVariant::fromValue(i)}
                        }),
                        QVariantMap({
                            {"_zab", QVariant::fromValue(zm_id)},
                            {"position", QVariant::fromValue(j)},
                            {"number", hm->number},
                            {"street", hm->street},
                            {"postal_code", hm->postalCode},
                            {"lat", hm->lat},
                            {"lon", hm->lon},
                            {"package", hm->package},
                            {"letter", hm->letter}
                        })
                        );
                    if(hm_id < 0)
                    {
                        qDebug() << "HouseNumber " << hm_id;
                        return false;
                    }

                }
            }
            qDebug() << "zabs" << zabs.size();
            qDebug() << "zab" << zm_id;
        }
    }

    return true;
}


bool HouseNumberTableModel::createTable()
{
    if (tableNotExists())
    {
        // Format the table name and foreign key reference directly into the query string
        QString queryString = QString(
                                  "CREATE TABLE IF NOT EXISTS %1 ("
                                  "postal_code, "
                                  "street TEXT, "
                                  "number TEXT, "
                                  "package INTEGER, "
                                  "letter INTEGER, "
                                  "lat REAL, "
                                  "lon REAL, "
                                  "_zab INTEGER, "
                                  "_routing_manager"
                                  "FOREIGN KEY(_routing_manager) REFERENCES %2(id)"
                                  "FOREIGN KEY(_zab) REFERENCES %2(id)"
                                  "PRIMARY KEY(postal_code, street, number)"
                                  ")"
                                  ).arg(m_tableName, ZabTableModelName);  // Substitute table names directly

        m_query.prepare(queryString);

        // Execute the query and check for success
        if (m_query.exec())
            return true;

        // Log the error if the query fails
        qFatal("Failed to create table: %s", qPrintable(m_query.lastError().text()));
    }

    return false;
}

bool ZabTableModel::createTable()
{
    if(tableNotExists())
    {
        // Format the table name and the foreign key reference directly into the query string
        QString queryString = QString(
                                  "CREATE TABLE IF NOT EXISTS %1 ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "position INTEGER NOT NULL, "
                                  "postal_code TEXT, "
                                  "street TEXT NOT NULL, "
                                  "_routing_manager INTEGER, "
                                  "FOREIGN KEY(_routing_manager) REFERENCES %2(id) ON DELETE CASCADE"
                                  ")"
                                  ).arg(m_tableName, RoutingManagerTableModelName); // Direct substitution of table names

        m_query.prepare(queryString);

        // Execute the query and check for success
        if (m_query.exec())
            return true;

        // Log the error if the query fails
        qFatal("Failed to create table: %s", qPrintable(m_query.lastError().text()));
    }

    return false;
}

bool RoutingManagerTableModel::createTable()
{
    if(tableNotExists())
    {
        // Format the table name directly into the query string
        QString queryString = QString(
                                  "CREATE TABLE IF NOT EXISTS %1("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "name TEXT NOT NULL,"
                                  "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP"
                                  ")"
                                  ).arg(m_tableName);  // Directly substitute the table name

        m_query.prepare(queryString);

        // Execute the query and check for success
        if (m_query.exec())
            return true;

        // Log the error if the query fails
        qFatal("Failed to create table: %s", qPrintable(m_query.lastError().text()));
    }

    return false;
}

HouseNumberTableModel::HouseNumberTableModel(QObject* parent) : TableModel(parent)
{
    m_tableName = HouseNumberTableModelName;
    setTable(HouseNumberTableModelName);
}

ZabTableModel::ZabTableModel(QObject* parent) : TableModel(parent)
{
    m_tableName = ZabTableModelName;
    setTable(ZabTableModelName);
}


RoutingManagerTableModel::RoutingManagerTableModel(QObject* parent) : TableModel(parent)
{
    m_tableName = RoutingManagerTableModelName;
    setTable(RoutingManagerTableModelName);
}


QObject* DataBase::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(m_instance==nullptr) { m_instance = new DataBase;}
    m_instance->select();
    return m_instance;
}
HouseNumberTableModel::
QObject* HouseNumberTableModel::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(m_instance==nullptr) { m_instance = new HouseNumberTableModel;}
    m_instance->select();
    return m_instance;
}

QObject* ZabTableModel::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(m_instance==nullptr) { m_instance = new ZabTableModel;}
    m_instance->select();
    return m_instance;
}

QObject* RoutingManagerTableModel::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(m_instance==nullptr) { m_instance = new RoutingManagerTableModel;}
    m_instance->select();
    return m_instance;
}

QString HouseNumberTableModel::tableName() const
{
    return HouseNumberTableModelName;
}

QString ZabTableModel::tableName() const
{
    return ZabTableModelName;
}

QString RoutingManagerTableModel::tableName() const
{
    return RoutingManagerTableModelName;
}


QSqlQuery DataBase::queryWithQVariantMap(DataBase::QUERY_TYPE t, const QVariantMap& map, const QString& tableName)
{
    QString queryString;
    const QStringList keys = map.keys();
    const QVariantList values = map.values();
    QSqlQuery query;
    switch (t) {
    case PostlerStorage::DataBase::SELECT:
        queryString += QString("SELECT * FROM %1 WHERE ").arg(tableName);
        PostlerStorage::DataBase::selectString(keys, tableName, queryString);
        query.prepare(queryString);
        PostlerStorage::DataBase::bindValues(keys, values, query);
        break;
    case PostlerStorage::DataBase::INSERT:
        queryString += QString("INSERT INTO %1 ").arg(tableName);
        PostlerStorage::DataBase::insertString(keys, tableName, queryString);
        query.prepare(queryString);
        PostlerStorage::DataBase::bindValues(values, query);
        break;
    case PostlerStorage::DataBase::UPDATE:
        queryString += QString("UPDATE %1").arg(tableName);
        PostlerStorage::DataBase::updateString(keys, tableName, queryString);
        query.prepare(queryString);
        PostlerStorage::DataBase::bindValues(values, query);
    default:
        break;
    }
    return query;
}

void DataBase::updateString(const QStringList& keys, const QString& tableName, QString& queryString)
{
    for(int i = 0; i < keys.size() - 1; i++)
    {
        if(i < keys.size() - 2)
        {
            queryString += QString(" %1 = :%1, ").arg(keys[i]);
        }
        else
        {
            queryString += QString(" WHERE id = :id");
        }
    }
}

void DataBase::selectString(const QStringList& keys, const QString& tableName, QString& queryString)
{
    for(int i = 0; i < keys.size(); i++)
    {
        queryString += QString("%1 = :%1").arg(keys[i]);
        if(i < keys.size() - 1) {
            queryString += " AND ";
        }
    }
}


void DataBase::insertString(const QStringList& keys, const QString& tableName, QString& queryString)
{
    QString keyList;
    QString questionMarkValues;
    for(int i = 0; i < keys.size(); i++)
    {
        keyList += keys[i];
        questionMarkValues += "?";
        if(i < keys.size() - 1) {
            keyList += ",";
            questionMarkValues += ",";
        }
    }
    queryString += QString("(%1) VALUES (%2)").arg(keyList, questionMarkValues);
}

void DataBase::bindValues(const QStringList& keys, const QVariantList& values, QSqlQuery& query)
{
    for(int i = 0; i < values.size(); i++)
    {
        QString key = QString(":%1").arg(keys[i]);
        query.bindValue(key, values[i]);
    }
}

void DataBase::bindValues(const QVariantList& values, QSqlQuery& query)
{
    for(int i = 0; i < values.size(); i++)
        query.bindValue(i, values[i]);
}
