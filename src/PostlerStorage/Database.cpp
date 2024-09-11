#include "include/PostlerStorage/Database.h"
#include "include/PostlerStorage/PostlerStorage.h"
#include "Constants.h"
#include "qregularexpression.h"
#include "qsqlerror.h"
#include "qsqlrecord.h"

using namespace PostlerStorage;

Database *Database::m_instance = nullptr;

void Database::init()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(POSTLER_STORAGE_PATH);
    if (!m_database.open()) {
        qDebug() << "Error: Failed to open or create the database: " << m_instance->m_database.lastError().text();
    } else {
        qDebug() << "Database opened or created successfully!";
    }
    m_query = new QSqlQuery(m_database);

    m_routeTable = initTable(m_routeTable, PostlerStorage::TABLENAME::ROUTE);
    m_zabTable = initTable(m_zabTable, PostlerStorage::TABLENAME::ZAB);
    m_houseNumberTable = initTable(m_houseNumberTable, PostlerStorage::TABLENAME::HOUSE_NUMBER);
    m_stopTable = initTable(m_stopTable, PostlerStorage::TABLENAME::STOP);
}

QVariant Database::convertToModels(const QString& text, const QString& splitText, NominatimRessource* res)
{

    int index = 0, zab = 0;
    QStringList lines = text.split(splitText);

    int r_id = m_routeTable->insert(QVariantMap({
        { SQL_VALUES::ROUTE::NAME, QDateTime::currentDateTime().toString() }
    }));

    if(r_id < 0) return QVariantMap({{"Error", QString("Routingmanager: %1").arg(r_id)}});

    for(const QString& line : lines)
    {
        if(!line.isEmpty()) {
            zab ++;
            index = 0;
            while(index ++ < line.size())
                if(line[index].isDigit())
                    break;
            QString street = line.sliced(0, index).simplified();
            // To warn the user
            regex.setPattern("^[a-zA-Z]");

            QStringList numbers = line.sliced(index).split(",");

            int zm_id = m_zabTable->insert(QVariantMap({
                                                        { SQL_VALUES::ZAB::NUMBER, zab },
                                                        { SQL_VALUES::STREET, street },
                                                        { SQL_VALUES::ZAB::ROUTE, r_id },
                                                        { SQL_VALUES::ZAB::POSTAL_CODE, splitText }
            }));
            if(zm_id < 0) return QVariantMap({{"Error", QString("ZabModel: %1").arg(zm_id)}});
            for(QString& number: numbers)
            {
                number = number.simplified();
                number.replace( " ", "" );

                int hm_id = m_houseNumberTable->insertAndCheck(QVariantMap({
                                    {SQL_VALUES::HOUSE_NUMBER::POSTAL_CODE, splitText },
                                    {SQL_VALUES::HOUSE_NUMBER::STREET , street },
                                    {SQL_VALUES::HOUSE_NUMBER::NUMBER, number}
                }), res);
                if(hm_id < 0) return QVariantMap({{"Error", QString("houseNumber: %1").arg(hm_id)}});

                m_stopTable->insert(QVariantMap({
                    { SQL_VALUES::STOP::HOUSE_NUMBER, hm_id },
                    { SQL_VALUES::STOP::ZAB, zm_id },
                    { SQL_VALUES::STOP::GOODS, 0 },
                    { SQL_VALUES::STOP::LETTER, 0 },
                    { SQL_VALUES::STOP::MAIL_BOX, 0 },
                    { SQL_VALUES::STOP::VERIFY, 0 },
                    { SQL_VALUES::STOP::PICKUP, 0 },
                    { SQL_VALUES::STOP::PACKAGE, 0 }
                }));

            }
        }
    }
    return QVariantMap({{"Error", false}, {"route_id", r_id}});
}

QObject* Database::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(m_instance==nullptr) { m_instance = new Database;}
    return m_instance;
}


QSqlQuery* Database::queryWithQVariantMap(int t, const QVariantMap& map, const QString& tableName)
{
    QString queryString;
    const QStringList keys = map.keys();
    const QVariantList values = map.values();
    QSqlQuery* query = new QSqlQuery();
    switch (t) {
    case PostlerStorage::SELECT:
        queryString += QString("SELECT * FROM %1 WHERE ").arg(tableName);
        PostlerStorage::Database::selectString(keys, tableName, queryString);
        query->prepare(queryString);
        PostlerStorage::Database::bindValues(keys, values, query);
        break;
    case PostlerStorage::INSERT:
        queryString += QString("INSERT INTO %1 ").arg(tableName);
        PostlerStorage::Database::insertString(keys, tableName, queryString);
        query->prepare(queryString);
        PostlerStorage::Database::bindValues(values, query);
        break;
    case PostlerStorage::UPDATE:
        queryString += QString("UPDATE %1 SET").arg(tableName);
        PostlerStorage::Database::updateString(keys, tableName, queryString);
        queryString += QString("WHERE id = :id");
        query->prepare(queryString);
        PostlerStorage::Database::bindValues(keys, values, query);
    default:
        break;
    }
    return query;
}

void Database::updateString(const QStringList& keys, const QString& tableName, QString& queryString)
{
    for(int i = 0; i < keys.size(); i++)
    {
        if(!keys[i].contains(SQL_VALUES::ID)){
            queryString += QString(" %1 = :%1 ").arg(keys[i]);
            if(i < keys.size() - 1) {
                queryString += ", ";
            }

        }
    }
}

void Database::selectString(const QStringList& keys, const QString& tableName, QString& queryString)
{
    for(int i = 0; i < keys.size(); i++)
    {
        queryString += QString("%1 = :%1").arg(keys[i]);
        if(i < keys.size() - 1) {
            queryString += " AND ";
        }
    }
}


void Database::insertString(const QStringList& keys, const QString& tableName, QString& queryString)
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

void Database::bindValues(const QStringList& keys, const QVariantList& values, QSqlQuery* query)
{
    for(int i = 0; i < values.size(); i++)
    {
        QString key = QString(":%1").arg(keys[i]);
        query->bindValue(key, values[i]);
    }
}

void Database::bindValues(const QVariantList& values, QSqlQuery* query)
{
    for(int i = 0; i < values.size(); i++)
        query->bindValue(i, values[i]);
}

void Database::setTempRoute(int tempId)
{

    /*

    qDebug() << m_zabTable->record(5).value("street");
    qDebug() << tempId;
    QString filter = QString("%1 = %2").arg(SQL_VALUES::ZAB::ROUTE, QString::number(tempId));
    qDebug() << filter;
    m_zabTable->setFilter(filter);
    m_zabTable->select();
    qDebug() << m_zabTable->rowCount();
*/
}

