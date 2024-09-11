
#ifndef OSMDATABASEMODEL_H
#define OSMDATABASEMODEL_H

#include <QObject>
#include <QQuickItem>
#include <QSqlTableModel>

namespace OSMDataBaseModel
{

    class OSMTableModel : public QSqlTableModel
    {
        Q_OBJECT

        Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged FINAL)
        Q_INVOKABLE void clear();
    public:
        OSMTableModel(QObject* parent = nullptr);

        const virtual QString tableName() = 0;
        virtual void createTable() = 0;
        virtual void insert(const QMap<QString, QString>&&) = 0;

        bool tableNotExists();

        QString filter() const {
            return m_filter;
        }

        void setFilter(const QString &filter) {
            if (m_filter != filter) {
                m_filter = filter;
                QSqlTableModel::setFilter(filter);
                emit filterChanged();
                select();  // Refresh the model with the new filter
            }
        }

    signals:
        void filterChanged();

    private:
        QString m_filter;


    };

    class TownModel : public OSMTableModel
    {
        Q_OBJECT


    public:
        TownModel(QObject* parent = nullptr);
        TownModel& operator=(const TownModel&);
        QVariant data(const QModelIndex &index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;

        const QString tableName() override;
        void createTable() override;
        void insert(const QMap<QString, QString>&&) override;
    };
    class StreetModel : public OSMTableModel
    {
        Q_OBJECT

    public:
        StreetModel(QObject* parent = nullptr);
        StreetModel& operator=(const StreetModel&);
        QVariant data(const QModelIndex &index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;

        const QString tableName() override;
        void createTable() override;
        void insert(const QMap<QString, QString>&&) override;
    };

    class HouseNumberModel : public OSMTableModel
    {
        Q_OBJECT

    public:
        HouseNumberModel(QObject* parent = nullptr);
        HouseNumberModel& operator=(const HouseNumberModel&);
        QVariant data(const QModelIndex &index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE QVariantList queryHouseNumberLocations(const QVariantList&);

        const QString tableName() override;
        void createTable() override;
        void insert(const QMap<QString, QString>&&) override
        {}
        void insert(const QVariantMap&& map);
    };

}


#endif // OSMDATABASEMODEL_H
