#include "include/PostlerStorage/Model/ZabTable.h"
#include "include/PostlerStorage/Database.h"
#include "include/PostlerStorage/PostlerStorage.h"
#include "qsqlerror.h"
#include "qsqlrecord.h"

using namespace PostlerStorage;

ZabTable::ZabTable(QObject *parent) : Table(parent)
{

}

QString ZabTable::tableName() const
{
    return PostlerStorage::TABLENAME::ZAB;
}

int ZabTable::insert(QVariant&& var)
{
    return insert(var);
}

int ZabTable::insert(QVariant& var)
{
    QVariantMap map = var.toMap();
    m_query = Database::queryWithQVariantMap(SELECT, map, tableName());

    if(m_query->exec())
    {
        if(m_query->next())
        {
            int id = m_query->value("id").toInt();
            map[SQL_VALUES::ID] = id;
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

bool ZabTable::createTable()
{
    QString queryString = QString(
                              "CREATE TABLE IF NOT EXISTS %1 (")
                              .arg(PostlerStorage::TABLENAME::ZAB);  // Directly substitute the table name
    queryString += QString("%1 %2 PRIMARY KEY AUTOINCREMENT,")
                       .arg(SQL_VALUES::ID, PostlerStorage::sql_T_ToString(SQL_T::INTEGER));

    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::ZAB::NUMBER, PostlerStorage::sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::ZAB::STREET, PostlerStorage::sql_T_ToString(SQL_T::TEXT));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::ZAB::POSTAL_CODE, PostlerStorage::sql_T_ToString(SQL_T::INTEGER));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::ZAB::PART, PostlerStorage::sql_T_ToString(SQL_T::TEXT));
    queryString += QString("%1 %2,")
                       .arg(SQL_VALUES::ZAB::ROUTE, PostlerStorage::sql_T_ToString(SQL_T::INTEGER));


    queryString += QString("FOREIGN KEY(%1) REFERENCES %2(%3)")
                       .arg(SQL_VALUES::ZAB::ROUTE, TABLENAME::ROUTE, SQL_VALUES::ID);

    queryString += ")";
    m_query->prepare(queryString);

    if (m_query->exec())
        return true;
    qDebug() << queryString;
    qFatal("Failed to create table: %s", qPrintable(m_query->lastError().text()));
}

QHash<int, QByteArray> ZabTable::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Part] = SQL_VALUES::ZAB::PART;
    roles[Route] = SQL_VALUES::ZAB::ROUTE;
    roles[PostalCode] = SQL_VALUES::ZAB::POSTAL_CODE;
    roles[Number] = SQL_VALUES::ZAB::NUMBER;
    roles[Street] = SQL_VALUES::ZAB::STREET;
    return roles;
}

QHash<int, int> ZabTable::roleTypes() const {
    QHash<int, int> roles;
    roles[Part] = SQL_T::TEXT;
    roles[Route] = SQL_T::INTEGER;
    roles[PostalCode] = SQL_T::TEXT;
    roles[Number] = SQL_T::INTEGER;
    roles[Street] = SQL_T::TEXT;
    return roles;
}


QVariant ZabTable::data(const QModelIndex &index, int role) const {
    //qDebug() << rowCount();
    if(index.isValid())
    {
        const QVariantMap var = m_tableEntitys[index.row()].toMap();
        //qDebug() << r;
        switch (role) {
        case Street:
            return var.value(SQL_VALUES::ZAB::STREET);
        default:
            return QVariant();
        }
    }

    return QVariant();

}

