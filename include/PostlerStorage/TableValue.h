#ifndef TABLEVALUE_H
#define TABLEVALUE_H

#include "include/PostlerStorage/PostlerStorage.h"
#include "qvariant.h"

namespace PostlerStorage {

struct TableValue{
    TableValue(QVariant v, SQL_T t, QString f = "", bool pk = false, bool fk = false) {
        value = v;
        type = t;
        primaryKey = pk;
        foreignKey = fk;
        foreignTable = f;
    }
    SQL_T type;
    QVariant value;
    bool primaryKey;
    bool foreignKey;
    QString foreignTable;
};

}

#endif // TABLEVALUE_H
