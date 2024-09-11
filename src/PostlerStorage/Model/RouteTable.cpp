#include "include/PostlerStorage/Model/RouteTable.h"
#include "include/PostlerStorage/Database.h"
#include "include/PostlerStorage/PostlerStorage.h"
#include "QString"
#include "qsqlerror.h"


using namespace PostlerStorage;

RouteTable::RouteTable(QObject *parent) : Table(parent)
{
}

QString RouteTable::tableName() const
{
    return PostlerStorage::TABLENAME::ROUTE;
}

int RouteTable::insert(QVariant&& var)
{
    return insert(var);
}

int RouteTable::insert(QVariant& var)
{
    QVariantMap map = var.toMap();
    m_query = Database::queryWithQVariantMap(SELECT, map, tableName());
    if(m_query->exec())
    {
        if(m_query->next())
        {
            int id = m_query->value("id").toInt();
            map[SQL_VALUES::ID] = id;

            QDateTime currentDateTime = QDateTime::currentDateTime();

            // Format the date and time as a string
            QString formattedTimestamp = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");
            map[SQL_VALUES::ROUTE::TIMESTAMP] = formattedTimestamp;

            m_query = Database::queryWithQVariantMap(UPDATE, map, tableName());
            if(m_query->exec())
            {
                return id;
            }
            else {
                qDebug() << m_query->lastError();
                return -2;
            }
        }
        else
        {
            m_query = Database::queryWithQVariantMap(INSERT, map, tableName());
            if(m_query->exec())
            {

                return m_query->lastInsertId().toInt();
            }
            else
            {
                return -3;
            }
        }
    }
    return -1;
}

bool RouteTable::createTable()
{
    QString queryString = QString(
                              "CREATE TABLE IF NOT EXISTS %1 (")
                              .arg(PostlerStorage::TABLENAME::ROUTE);  // Directly substitute the table name
    queryString += QString("%1 %2 PRIMARY KEY AUTOINCREMENT,")
                       .arg(SQL_VALUES::ID, sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::ROUTE::NAME, sql_T_ToString(SQL_T::TEXT));
    queryString += QString("%1 %2 DEFAULT CURRENT_TIMESTAMP")
                       .arg(SQL_VALUES::ROUTE::TIMESTAMP, sql_T_ToString(SQL_T::DATETIME));

    queryString += ")";
    m_query->prepare(queryString);

    if (m_query->exec())
        return true;
    else
        qDebug() << queryString;
        qFatal("Failed to create table: %s", qPrintable(m_query->lastError().text()));
}

QHash<int, QByteArray> RouteTable::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = SQL_VALUES::ROUTE::NAME;
    roles[TimeStamp] = SQL_VALUES::ROUTE::TIMESTAMP;
    return roles;
}

QHash<int, int> RouteTable::roleTypes() const {
    QHash<int, int> roles;
    roles[Name] = SQL_T::TEXT;
    roles[TimeStamp] = SQL_T::DATETIME;
    return roles;
}

QVariant RouteTable::data(const QModelIndex &index, int role) const {
    //qDebug() << rowCount();
    if(index.isValid())
    {

    }

    return QVariant();

}

int RouteTable::rowCount(const QModelIndex& index) const
{
    return 0;
}
