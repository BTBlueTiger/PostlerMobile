#include "include/PostlerStorage/Model/StopTable.h"
#include "include/PostlerStorage/Database.h"
#include "include/PostlerStorage/PostlerStorage.h"
#include "qsqlerror.h"
using namespace PostlerStorage;


StopTable::StopTable(QObject *parent) : Table(parent)
{
}


QString StopTable::tableName() const
{
    return PostlerStorage::TABLENAME::ZAB;
}

int StopTable::insert(QVariant&& var)
{
    return insert(var);
}

int StopTable::insert(QVariant& var)
{
    QVariantMap map = var.toMap();
    m_query = Database::queryWithQVariantMap(SELECT, map, tableName());
    if(m_query->exec())
    {
        if(m_query->next())
        {
            return m_query->value("id").toInt();
        }
        else
        {
            m_query = Database::queryWithQVariantMap(INSERT, map, tableName());
            if(m_query->exec())
            {
                return m_query->value("id").toInt();
            }
        }
    }
    return -1;
}

bool StopTable::createTable()
{
    QString queryString = QString(
                              "CREATE TABLE IF NOT EXISTS %1 (")
                              .arg(PostlerStorage::TABLENAME::STOP);  // Directly substitute the table name
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::STOP::ID, sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::STOP::PACKAGE, sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::STOP::LETTER, sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::STOP::GOODS, sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::STOP::VERIFY, sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::STOP::MAIL_BOX, sql_T_ToString(SQL_T::INTEGER));

    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::STOP::ROUTE, sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::STOP::HOUSE_NUMBER, sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::STOP::ZAB, sql_T_ToString(SQL_T::INTEGER));

    queryString += QString("FOREIGN KEY(%1) REFERENCES %2(%3),")
                       .arg(SQL_VALUES::STOP::HOUSE_NUMBER, TABLENAME::HOUSE_NUMBER, SQL_VALUES::ID);
    queryString += QString("FOREIGN KEY(%1) REFERENCES %2(%3)")
                       .arg(SQL_VALUES::STOP::ZAB, TABLENAME::ZAB, SQL_VALUES::ID);

    queryString += QString("FOREIGN KEY(%1) REFERENCES %2(%3) ON DELETE CASCADE")
                       .arg(SQL_VALUES::STOP::ROUTE, TABLENAME::ROUTE, SQL_VALUES::ID);

    queryString += ")";
    m_query->prepare(queryString);

    if (m_query->exec())
        return true;
    qDebug() << queryString;
    qFatal("Failed to create table: %s", qPrintable(m_query->lastError().text()));
}

QHash<int, QByteArray> StopTable::roleNames() const {
    QHash<int, QByteArray> roles;
    return roles;
}

QHash<int, int> StopTable::roleTypes() const {
    QHash<int, int> roles;
    return roles;
}

QVariant StopTable::data(const QModelIndex &index, int role) const {
    //qDebug() << rowCount();
    if(index.isValid())
    {

    }

    return QVariant();
}

int StopTable::rowCount(const QModelIndex& index) const
{
    return 0;
}
