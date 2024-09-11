#include "include/PostlerStorage/Model/Table.h"
#include "include/PostlerStorage/Database.h"
#include "include/PostlerStorage/PostlerStorage.h"
#include "qjsonvalue.h"
#include "qjsvalue.h"
#include "qsqlerror.h"

using namespace PostlerStorage;


Table::Table(QObject* parent) : QAbstractListModel(parent)
{

}

int Table::insert(QVariant&& var)
{
    return insert(var);
}


void Table::setDataBase(QSqlDatabase* db)
{
    m_dataBase = db;
    m_query = new QSqlQuery(*db);
}

void Table::clearFilter()
{
    m_filter.clear();
}

void Table::addFilter(const QVariantMap& var)
{
    m_filter.append(var);
    qDebug() << var;

    QString filterString = QString("SELECT * FROM %1 WHERE ").arg(tableName());
    QHash<int, QByteArray> rNames(roleNames());
    int i = 0;

    for(const QVariant& var: m_filter)
    {
        i++;
        QVariantMap map = var.toMap();
        QString role = QString::fromUtf8(rNames[map[TABLE_FILTER::TABLE_ROLE].toInt()]);


        switch (map[TABLE_FILTER::FILTER_T].toInt()) {
        case Database::Filter_T::Equation:
            filterString += QString("%1 = ?").arg(role);
            break;
        case Database::Filter_T::Like:
            filterString += QString("%1 LIKE ?").arg(role);
            break;
        default:
            break;
        }
        if(i < m_filter.size()) {
            filterString += " AND ";
        }
    }

    m_query = new QSqlQuery(*m_dataBase);
    m_query->prepare(filterString);

    for(int i = 0; i < m_filter.size(); i++)
    {
        m_query->bindValue(i, m_filter[0].toMap()[TABLE_FILTER::VALUE]);
    }
    m_query->exec();
  /*
    m_query = new QSqlQuery(*m_dataBase);
    m_query->prepare("SELECT * FROM ZAB WHERE _route = 1");
    m_query->bindValue(":_route", 1);

    for(const QVariant& var : keyAndVals)
    {
        QVariantMap map = var.toMap();
        QString role = QString::fromUtf8(rNames[map[TABLE_FILTER::TABLE_ROLE].toInt()]);

        m_query->bindValue(":_route", 1);
    }
*/
    qDebug() << m_query->lastQuery();
    qDebug() << m_query->boundValues();
    qDebug() << m_query->lastError();
    /*
    qDebug() << var;
    QString role = QString::fromUtf8(roleNames()[var[TABLE_FILTER::TABLE_ROLE].toInt()]);
    QString filterString = QString("SELECT * FROM %1 WHERE %2 = ?").arg(tableName(), role);
    qDebug() << m_dataBase;
    m_query = new QSqlQuery(*m_dataBase);
    m_query->prepare(filterString);
    m_query->bindValue(0, var[TABLE_FILTER::VALUE].toInt());

    m_query->exec();
    qDebug() << m_query->lastError();
*/
}



int Table::rowCount(const QModelIndex &parent) const
{
    return m_tableEntitys.size();
}

void Table::select()
{
    QVariantMap val;
    QHash<int, QByteArray> rNames = roleNames();
    m_tableEntitys.clear();
    while (m_query->next()) {

        QMutableHashIterator<int, QByteArray> i(rNames);
        val.clear();
        while (i.hasNext()) {
            i.next();

            QString key = i.value();
            val[key] = m_query->value(key);
        }
        m_tableEntitys.append(val);
    }
}
