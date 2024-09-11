#ifndef ROUTETABLE_H
#define ROUTETABLE_H


#include "QMetaEnum"
#include "include/PostlerStorage/Model/Table.h"


namespace PostlerStorage {

    class RouteTable : public Table
    {
        Q_OBJECT
        Q_PROPERTY(QString name MEMBER m_name FINAL)
    public:
        RouteTable(QObject *parent = nullptr);

        enum TABLE_ROLES {
            Name = Qt::UserRole +1,
            TimeStamp
        };

        Q_ENUM(TABLE_ROLES);

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;
        QHash<int, int> roleTypes() const override;


        QString tableName() const override;
        bool createTable() override;
        int insert(QVariant&) override;
        int insert(QVariant&&) override;

    private:
        QString m_name;
    };

}

#endif // ROUTETABLE_H
