#include "include/PostlerStorage/Model/HouseNumberTable.h"
#include "include/PostlerStorage/PostlerStorage.h"
#include "qsqlerror.h"
#include "../../../include/PostlerStorage/Database.h"
#include "qsqlrecord.h"

using namespace PostlerStorage;

HouseNumberTable::HouseNumberTable(QObject *parent) : Table(parent)
{

}

QString HouseNumberTable::tableName() const
{
    return PostlerStorage::TABLENAME::HOUSE_NUMBER;
}

void HouseNumberTable::checkNominatim(NominatimRessource* res, const QVariantMap& map, qlonglong id)
{
    QString queryString = QString("search?q=%1 %2 %3").arg(
        map[SQL_VALUES::HOUSE_NUMBER::POSTAL_CODE].toString(),
        map[SQL_VALUES::HOUSE_NUMBER::STREET].toString(),
        map[SQL_VALUES::HOUSE_NUMBER::NUMBER].toString()
        );
    QNetworkReply* reply = res->createQNetworkReply(queryString);
    connect(reply, &QNetworkReply::finished, this, [=] {
        update(reply, id);
    });
}

int HouseNumberTable::insert(QVariant&)
{
    return -10;
}

int HouseNumberTable::insert(QVariant&&)
{
    return -10;
}

int HouseNumberTable::insertAndCheck(const QVariant& variant, NominatimRessource *res)
{
    QVariantMap map = variant.toMap();
    m_query = Database::queryWithQVariantMap(SELECT, map, tableName());
    qlonglong entryID = -1;
    if(m_query->exec())
    {
        if(m_query->next())
        {
            entryID = m_query->value("id").toULongLong();
            if(m_query->value(SQL_VALUES::HOUSE_NUMBER::STATE_QUERY).toInt() < 0)
                checkNominatim(res, map, entryID);
            return entryID;
        }
        else
        {
            map[SQL_VALUES::HOUSE_NUMBER::STATE_QUERY] = -1;
            m_query = Database::queryWithQVariantMap(INSERT, map, tableName());
            if(m_query->exec())
            {
                entryID = m_query->lastInsertId().toULongLong();
                checkNominatim(res, map, entryID);
                return entryID;
            }
        }
    }
    else
    {
        qDebug() << "Query failed";
    }
    return entryID;
}

void HouseNumberTable::update(QNetworkReply* reply, qlonglong id)
{
    if(reply->isFinished())
    {
        QVariantMap toUpdate;
        if(reply->error()) {
            qDebug() << "error";
            qDebug() << reply->errorString();
            toUpdate[SQL_VALUES::HOUSE_NUMBER::STATE_QUERY] = -1;
        }
        else
        {
            QByteArray responseData = reply->readAll();
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            const QJsonArray jsonArray = jsonDoc.array();

            if (jsonArray.isEmpty()) {
                toUpdate[SQL_VALUES::HOUSE_NUMBER::STATE_QUERY] = 0;
            }
            else
            {
                if(jsonArray.size() > 1) {
                    toUpdate[SQL_VALUES::HOUSE_NUMBER::STATE_QUERY] = 2;
                } else {
                    toUpdate[SQL_VALUES::HOUSE_NUMBER::STATE_QUERY] = 1;
                }
                QJsonValue v = jsonArray.first();
                toUpdate[SQL_VALUES::HOUSE_NUMBER::LAT] = v["lat"].toString().toDouble();
                toUpdate[SQL_VALUES::HOUSE_NUMBER::LON] = v["lon"].toString().toDouble();

            };

        }
        toUpdate[SQL_VALUES::ID] = (int)id;
        m_query = Database::queryWithQVariantMap(UPDATE, toUpdate, tableName());
        if(m_query->exec())
        {
            qDebug() << "Worked";
        }
        else
        {
            qDebug() << m_query->lastError();
            qDebug() << "Not worked";
        }
    }
}

bool HouseNumberTable::createTable()
{
    QString queryString = QString(
                              "CREATE TABLE IF NOT EXISTS %1 (")
                              .arg(PostlerStorage::TABLENAME::HOUSE_NUMBER);  // Directly substitute the table name
    queryString += QString("%1 %2 PRIMARY KEY AUTOINCREMENT,")
                       .arg(SQL_VALUES::ID, PostlerStorage::sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::POSTAL_CODE, PostlerStorage::sql_T_ToString(SQL_T::TEXT));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::HOUSE_NUMBER::STREET, sql_T_ToString(SQL_T::TEXT));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::HOUSE_NUMBER::NUMBER, sql_T_ToString(SQL_T::TEXT));

    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::HOUSE_NUMBER::LAT, sql_T_ToString(SQL_T::REAL));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::HOUSE_NUMBER::LON, sql_T_ToString(SQL_T::REAL));
    queryString += QString("%1 %2")
                       .arg(SQL_VALUES::HOUSE_NUMBER::STATE_QUERY, sql_T_ToString(SQL_T::INTEGER));

    queryString += ")";
    m_query->prepare(queryString);

    if (m_query->exec())
        return true;
    qDebug() << queryString;
    qFatal("Failed to create table: %s", qPrintable(m_query->lastError().text()));
}

QHash<int, QByteArray> HouseNumberTable::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PostalCode] = SQL_VALUES::ROUTE::NAME;
    roles[Street] = SQL_VALUES::ROUTE::TIMESTAMP;
    roles[Number] = SQL_VALUES::ROUTE::NAME;
    roles[Lat] = SQL_VALUES::ROUTE::NAME;
    roles[Lon] = SQL_VALUES::ROUTE::TIMESTAMP;
    return roles;
}

QHash<int, int> HouseNumberTable::roleTypes() const {
    QHash<int, int> roles;
    roles[PostalCode] = SQL_T::INTEGER;
    roles[Street] = SQL_T::TEXT;
    roles[Number] = SQL_T::TEXT;
    roles[Lat] = SQL_T::REAL;
    roles[Lon] = SQL_T::REAL;
    return roles;
}

QVariant HouseNumberTable::data(const QModelIndex &index, int role) const {
    //qDebug() << rowCount();
    if(index.isValid())
    {

    }

    return QVariant();
}

int HouseNumberTable::rowCount(const QModelIndex& index) const
{
    return 0;
}
