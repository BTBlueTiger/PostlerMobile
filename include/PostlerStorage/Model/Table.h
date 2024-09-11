#ifndef TABLEMODEL_H
#define TABLEMODEL_H


#include "qjsvalue.h"
#include "qsqlquery.h"
#include <QSqlTableModel>

namespace PostlerStorage {

    class Table : public QAbstractListModel
    {
        Q_OBJECT
    public:

        Table(QObject* parent = nullptr);
        virtual QString tableName() const = 0;
        virtual bool createTable() = 0;
        virtual int insert(QVariant&) = 0;
        virtual int insert(QVariant&&);
        virtual QHash<int, int> roleTypes() const = 0;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        void setDataBase(QSqlDatabase* db);
        Q_INVOKABLE void addFilter(const QVariantMap&);
        Q_INVOKABLE void clearFilter();
        Q_INVOKABLE virtual void select();

    protected:
        QSqlDatabase * m_dataBase;
        QSqlQuery* m_query;
        QVariantList m_filter;
        QVariantList m_tableEntitys;
    };
}

#endif // TABLEMODEL_H
