#ifndef HOUSENUMBER_H
#define HOUSENUMBER_H
#include "include/PostlerStorage/Model/Table.h"
#include "modules/NominatimRessource/NominatimRessource.h"
#include "include/PostlerStorage/PostlerStorage.h"

namespace PostlerStorage {

class HouseNumberTable : public Table {
    Q_OBJECT

public:
    HouseNumberTable(QObject *parent = nullptr);

    enum TABLE_ROLES {
        PostalCode = Qt::UserRole + 1,
        Street,
        Number,
        Lat,
        Lon,
    };

    Q_ENUM(TABLE_ROLES);

    QString tableName() const override;
    bool createTable() override;
    int insert(QVariant&) override;
    int insert(QVariant&&) override;
    int insertAndCheck(const QVariant& variant, NominatimRessource* res);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QHash<int, int> roleTypes() const override;

private:
    void checkNominatim(NominatimRessource* res, const QVariantMap& map, qlonglong id);
    void update(QNetworkReply* reply, qlonglong id);

};

};


#endif // HOUSENUMBER_H
