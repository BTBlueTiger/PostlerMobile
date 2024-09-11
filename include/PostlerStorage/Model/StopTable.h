#ifndef STOPTABLE_H
#define STOPTABLE_H

#include "include/PostlerStorage/Model/Table.h"
#include <QSqlQueryModel>

namespace PostlerStorage {
class StopTable: public Table {
    Q_OBJECT

public:
    StopTable(QObject *parent = nullptr);

    QString tableName() const override;
    bool createTable() override;
    int insert(QVariant& variant) override;
    int insert(QVariant&& var) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QHash<int, int> roleTypes() const override;

};

}

#endif // STOPTABLE_H
