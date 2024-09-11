#ifndef DATABASE_H
#define DATABASE_H

#include "include/PostlerStorage/Model/StopTable.h"
#include "include/PostlerStorage/Model/RouteTable.h"
#include "include/PostlerStorage/Model/ZabTable.h"
#include "include/PostlerStorage/Model/HouseNumberTable.h"
#include "modules/NominatimRessource/NominatimRessource.h"
#include "qjsengine.h"
#include "qobjectdefs.h"
#include "qqmlengine.h"
#include "qregularexpression.h"
#include "qsqldatabase.h"

namespace PostlerStorage {

class Database : public QObject {
    Q_OBJECT
    Database(QObject *parent = nullptr) : QObject(parent){}
    Q_PROPERTY(RouteTable* routeTable MEMBER m_routeTable FINAL)
    Q_PROPERTY(ZabTable* zabTable MEMBER m_zabTable FINAL)
    Q_PROPERTY(HouseNumberTable* houseNumberTable MEMBER m_houseNumberTable FINAL)
    Q_PROPERTY(StopTable* stopTable MEMBER m_stopTable FINAL)



public:

    enum Filter_T {
        Equation,
        Like
    };
    Q_ENUM(Filter_T)

    Q_INVOKABLE void init();
    Q_INVOKABLE void setTempRoute(int id);
    Q_INVOKABLE QVariant convertToModels(const QString& text, const QString& splitText, NominatimRessource *res);

    static Database *m_instance;
    static QObject* createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    static QSqlQuery* queryWithQVariantMap(int t , const QVariantMap& map, const QString& tableName);

private:

    template<typename T> T* initTable(T*& table, const QString& tableName)
    {
        table = new T(this);  // Allocate memory for the table
        table->setDataBase(&m_database);// Set the table name
        table->createTable();  // Create the table using the query
        return table;  // Return the pointer to the table
    }

    void insert();
    int insertOrUpdate(QVariantMap&& values);
    int insertOrUpdate(QVariantMap&& checkValues, QVariantMap&& values);

    int _insertOrUpdate(QVariantMap& values);
    int updateRecord(QVariantMap& values, int recordID);
    int insertRecord(const QVariantMap& values);
    int dataBaseTransactionWith_m_query();

    static void selectString(const QStringList& keys, const QString& tableName, QString& queryString);
    static void insertString(const QStringList& keys, const QString& tableName, QString& queryString);
    static void updateString(const QStringList&keys, const QString& tableName, QString& queryString);
    static void bindValues(const QVariantList& values, QSqlQuery* query);
    static void bindValues(const QStringList& keys, const QVariantList& values, QSqlQuery* query);

    QSqlDatabase m_database;
    RouteTable* m_routeTable;
    ZabTable* m_zabTable;
    HouseNumberTable* m_houseNumberTable;
    StopTable* m_stopTable;
    QSqlQuery* m_query;
    QRegularExpression regex;
};

}

#endif // DATABASE_H
