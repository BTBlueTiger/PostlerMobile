#ifndef ROUTINGMANAGER_H
#define ROUTINGMANAGER_H

#include "QVariantMap"
#include "modules/NominatimRessource/NominatimRessource.h"
#include "qabstractitemmodel.h"
#include <QGeoCoordinate>

class ZabModel;
class RoutingManager;


struct HousNumber : public RequestReceiver
{
    Q_OBJECT
public:

    enum ZabStopStates {
        DEFAULT,
        VALID_ONE,
        VALID_MANY,
        EMPTY,
        ERROR,
    };

    Q_ENUM(ZabStopStates)

    /*
    HousNumber(QObject *parent = nullptr)
        : NominatimReplyReceiver(parent) {}
    HousNumber(const QString& n, QObject *parent = nullptr)
        : NominatimReplyReceiver(parent), number(n) {}
*/
    ZabModel* zabModel;
    QString street, number, postalCode;
    QJsonArray requestResult;
    int row;
    bool package = false, letter = false;
    double lat, lon;
    int zabStopState;

    QVariant asQVariant();

    void ceckStop(NominatimRessource*);

public slots:
    void requestFinished(QNetworkReply* reply) override;

};

Q_DECLARE_METATYPE(HousNumber)

class ZabModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(QString postalCode READ postalCode WRITE setPostalCode NOTIFY streetChanged)
    Q_PROPERTY(QString street READ street WRITE setStreet NOTIFY streetChanged)
    Q_PROPERTY(bool areHouseNumbersChecked MEMBER m_areHouseNumbersChecked NOTIFY areHouseNumbersCheckedChanged FINAL)

public:
    explicit ZabModel(QObject *parent = nullptr);

    // Enum for roles
    enum ZabRoles {
        StreetRole = Qt::UserRole + 1,
        NumberRole,
        PackageRole,
        LetterRole,
        LatRole,
        LonRole,
        ZabStopStateRole
    };

    void zabStopChanged(int row);

    // Required overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    void updateRowIndices(int startIndex);

    // Custom functions to manipulate the list
    Q_INVOKABLE void addZabStop(HousNumber *zabStop);
    Q_INVOKABLE void addZabStop(int index);
    Q_INVOKABLE void removeZabStop(int index);
    Q_INVOKABLE void checkStops(NominatimRessource*);


    QVector<HousNumber*> houseNumbers();
    QString street() const;
    QString postalCode() const;

    void setStreet(const QString&);
    void setPostalCode(const QString&);
    void incrementCheckedZabs();
    void setRoutingManager(RoutingManager*);

    HousNumber* getZabStop(int index) {
        // Optional: You may add boundary checks to ensure 'index' is within range
        if (index < 0 || index >= m_zabStops.size()) {
            return nullptr; // Or handle error appropriately
        }
        return m_zabStops[index];
    }

private:
    RoutingManager* m_routingManager;
    QVector<HousNumber*> m_zabStops;
    QString m_street, m_postalCode;
    int m_checked_zabs = 0;
    bool m_areHouseNumbersChecked = false;


signals:
    void streetChanged();
    void postalCodeChanged();
    void areHouseNumbersCheckedChanged(bool);
};

Q_DECLARE_METATYPE(ZabModel)

class RoutingManager : public QAbstractListModel
{
    Q_OBJECT
    // m_query.exec()
    /*
            m_query = DataBase::queryWithQVariantMap(DataBase::SELECT, checkValues, tableName);
            if(instance->m_query.exec())
            {
                if(instance->m_query.next())
                {
                    return instance->m_query.value("id").toInt();
                }
                else
                {
                    instance->m_query = DataBase::queryWithQVariantMap(DataBase::INSERT, values, tableName);
                    instance->database().transaction();
                    if(instance->m_query.exec()){
                        instance->database().commit();
                        return instance->m_query.lastInsertId().toInt();
                    }
                    else
                    {
                        qDebug() << "Exec Failed:" << instance->m_query.lastError() << "\n with: " << instance->m_query.lastQuery();
                        instance->database().rollback();
                    }

                }// m_query.next())
                qDebug() << "Exec Failed:" << instance->m_query.lastError() << "\n with: " << instance->m_query.lastQuery();
            } // instance->m_query.exec()
            qDebug() << "Exec Failed:" << instance->m_query.lastError() << "\n with: " << instance->m_query.lastQuery();
        }
        else
        {
            qDebug() << "Instance is null";
        }
        return -1;
*/
    //Q_PROPERTY(QVariant route READ route WRITE setRoute NOTIFY routeChanged FINAL)
    Q_PROPERTY(QVariant cur_stop READ cur_stop NOTIFY cur_stopChanged FINAL)
    Q_PROPERTY(QVariant next_stop READ next_stop NOTIFY next_stopChanged FINAL)
    Q_PROPERTY(QVariant prev_stop READ prev_stop NOTIFY prev_stopChanged FINAL)
    Q_PROPERTY(QVariant waypoints MEMBER m_waypoints NOTIFY waypointsChanged FINAL)

    Q_PROPERTY(bool allZabsChecked MEMBER m_allZabsChecked NOTIFY allZabsCheckedChanged FINAL)

    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged FINAL)
    Q_PROPERTY(int nstops MEMBER m_n_stops NOTIFY nStopsChanged FINAL)
    Q_PROPERTY(int currentZab MEMBER m_currentZab  NOTIFY currentZabChanged FINAL)

    //Q_PROPERTY(QVariantList zabs READ zabs WRITE setZabs NOTIFY zabsChanged FINAL)

public:
    Q_INVOKABLE void next();
    Q_INVOKABLE void back();
    Q_INVOKABLE void nextZab();
    Q_INVOKABLE void backZab();

    Q_INVOKABLE void setWayPoints(int amount);

    Q_INVOKABLE void removeZab(int index);

    enum ZabRoles {
        ZabRole = Qt::UserRole + 1
        // Add more roles if needed
    };

    enum ActiveWayPoints {
        NEXT_N,
        ZAB,
        IMPORTANT
    };

    RoutingManager(QObject* parent = nullptr);
    RoutingManager& operator=(const RoutingManager& other);

    QVariant route() const;
    QVariant cur_stop() const;
    QVariant next_stop() const;
    QVariant prev_stop() const;

    int index() const;

    Q_INVOKABLE void textToZabs(const QVariant&);
    Q_INVOKABLE int checkZabs(NominatimRessource* res);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setRoute(const QVariant& newRoute);
    void setIndex(int);
    Q_INVOKABLE QVector<ZabModel*> zabs() const;

    //void setZabs(const QVariant&);
    //QVariantList zabs() const;

private:
    QVariant getStop(int) const;
    QVector<ZabModel*> m_zabs;
    QVariant m_waypoints;
    int m_n_stops = 0;
    int m_currentZab = 0;
    int m_checkedZabs = 0;
    bool m_allZabsChecked = false;
    QVariant m_route;
    int m_index;
    bool isNewRoute;

signals:
    void routeChanged();
    void coordinateChanged();
    void indexChanged();
    void next_stopChanged();
    void prev_stopChanged();
    void cur_stopChanged();
    void zabsChanged();
    void nStopsChanged();
    void currentZabChanged();
    void waypointsChanged();
    void allZabsCheckedChanged();

public slots:
    void zabCheckedChanged(bool);

private slots:
    void notifyIndexChanged();


};

#endif // ROUTINGMANAGER_H
