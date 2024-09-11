#ifndef ZABTABLE_H
#define ZABTABLE_H
#include "include/PostlerStorage/Model/Table.h"
#include "include/PostlerStorage/PostlerStorage.h"

namespace PostlerStorage {

struct Zab {
    QString street, part, postalCode, number;
    int route;

};

class ZabTable : public Table
{
    Q_OBJECT
public:
    ZabTable(QObject *parent = nullptr);

    enum TABLE_ROLES {
        Part = Qt::UserRole + 1,
        Route,
        PostalCode,
        Number,
        Street
    };

    Q_ENUM(TABLE_ROLES);


    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QHash<int, int> roleTypes() const override;

    QString tableName() const override;
    bool createTable() override;
    int insert(QVariant& variant) override;
    int insert(QVariant&& variant) override;
};
};

#endif // ZABTABLE_H
