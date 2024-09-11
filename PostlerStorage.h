#ifndef POSTLERSTORAGE_H
#define POSTLERSTORAGE_H

#include "qsqltablemodel.h"
#include <QObject>
#include <QSqlQuery>

#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <RoutingManager.h>

#include <QFileInfo>

namespace PostlerStorage {


constexpr const char* HouseNumberTableModelName = "house_number";
constexpr const char* ZabTableModelName = "zab";
constexpr const char* RoutingManagerTableModelName = "routing_manager";


class DataBase : public QSqlTableModel
{
    Q_OBJECT
public:
    DataBase();

    enum QUERY_TYPE{
        SELECT,
        INSERT,
        UPDATE
    };

    static QSqlQuery queryWithQVariantMap(QUERY_TYPE t , const QVariantMap& map, const QString& tableName);



    Q_INVOKABLE bool open();

    Q_INVOKABLE bool toDatabase(const RoutingManager*, const QString& name) const;
    Q_INVOKABLE RoutingManager toRoutingManager() const{ return nullptr;}


    static DataBase *m_instance;
    static QObject* createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

private:
    QSqlDatabase m_postlerDatabase;
    QSqlQuery m_query;

    static void selectString(const QStringList& keys, const QString& tableName, QString& queryString);
    static void insertString(const QStringList& keys, const QString& tableName, QString& queryString);
    static void updateString(const QStringList&keys, const QString& tableName, QString& queryString);
    static void bindValues(const QVariantList& values, QSqlQuery& query);
    static void bindValues(const QStringList& keys, const QVariantList& values, QSqlQuery& query);
};


class TableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    TableModel(QObject* parent = nullptr);

    Q_INVOKABLE virtual bool createTable() = 0;
    virtual QString tableName() const = 0;

    int insert(QVariantMap&& values)
    {
        m_query = DataBase::queryWithQVariantMap(DataBase::SELECT, values, tableName());
        if(m_query.exec())
        {
            return m_query.next() ? m_query.value("id").toInt() : insertRecord(values);
        }
        qDebug() << "Query exec Failed:" << m_query.lastError() << "\n with: " << m_query.lastQuery();
        return -1;
        //
    }

    int insertOrUpdate(QVariantMap&& values)
    {
        m_query = DataBase::queryWithQVariantMap(DataBase::SELECT, values, tableName());
        return _insertOrUpdate(values);
    }

    int insertOrUpdate(QVariantMap&& checkValues, QVariantMap&& values)
    {
        m_query = DataBase::queryWithQVariantMap(DataBase::SELECT, checkValues, tableName());
        return _insertOrUpdate(values);
    }

    bool tableNotExists();

protected:
    QSqlQuery m_query;
    QSqlRecord currentRecord;
    QSqlQueryModel m_queryModel;
    QString m_tableName = 0;

private:

    int _insertOrUpdate(QVariantMap& values)
    {
        if(m_query.exec())
        {
            return m_query.next() ? m_query.value("id").toInt() : updateRecord(values, m_query.value("id").toInt());
            //return m_query.next() ? updateRecord(values, m_query.value("id").toInt()) : insertRecord(values);
        }
        else
        {
            qDebug() << "Query exec Failed:" << m_query.lastError() << "\n with: " << m_query.lastQuery();
            return -1;
        }
    }

    int updateRecord(QVariantMap& values, int recordID)
    {
        //values["id"] = recordID;
        qDebug() << values;
        m_query = DataBase::queryWithQVariantMap(DataBase::UPDATE, values, tableName());
        return dataBaseTransactionWith_m_query();
    }

    int insertRecord(const QVariantMap& values)
    {
        m_query = DataBase::queryWithQVariantMap(DataBase::INSERT, values, tableName());
        return dataBaseTransactionWith_m_query();
    }

    int dataBaseTransactionWith_m_query()
    {
        database().transaction();
        if(m_query.exec())
        {
            database().commit();
            return m_query.value("id").toInt();
        }
        else
        {
            qDebug() << "exec Failed:" << m_query.lastError() << "\n with: " << m_query.lastQuery();
            database().rollback();
            return -1;
        }
    }
};


class HouseNumberTableModel : public TableModel
{
    Q_OBJECT
public:
    HouseNumberTableModel(QObject* parent = nullptr);
    bool createTable() override;
    QString tableName() const override;

    static HouseNumberTableModel *m_instance;
    static QObject* createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
protected:

};

class ZabTableModel : public TableModel
{
    Q_OBJECT
public:
    ZabTableModel(QObject* parent = nullptr);
    bool createTable() override;
    QString tableName() const override;

    static ZabTableModel *m_instance;
    static QObject* createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
};


class RoutingManagerTableModel : public TableModel
{
    Q_OBJECT
public:
    RoutingManagerTableModel(QObject* parent = nullptr);
    bool createTable() override;
    QString tableName() const override;

    static RoutingManagerTableModel *m_instance;
    static QObject* createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
};





}

#endif // ROUTINGSTORAGE_H
