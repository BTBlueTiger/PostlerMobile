#include "OsmDataBaseModel.h"
#include "qsqlquery.h"
#include "qsqlerror.h"
#include "qsqlrecord.h"

namespace OSMDataBaseModel
{

OSMTableModel::OSMTableModel(QObject* parent) : QSqlTableModel(parent)
    {
    }

    TownModel::TownModel(QObject* parent)
        : OSMTableModel(parent) {setTable("Town");}
    StreetModel::StreetModel(QObject* parent)
        : OSMTableModel(parent) {setTable("Street");}
    HouseNumberModel::HouseNumberModel(QObject* parent)
        : OSMTableModel(parent) {setTable("HouseNumber");}

    bool OSMTableModel::tableNotExists()
    {
        if (QSqlDatabase::database().tables().contains(tableName())) {
            // The table already exists; we don't need to do anything.
            return false;
        }
        return true;
    }

    TownModel& TownModel::operator=(const TownModel& townModel)
    {
        if (this == &townModel)
            return *this;
        return *this;
    }

    const QString TownModel::tableName()
    {
        return "Town";
    }

    void TownModel::createTable()
    {
        if(tableNotExists())
        {
            QSqlQuery m_query;
            if(!m_query.exec(
                "CREATE TABLE IF NOT EXISTS Town ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "city TEXT,"
                "postcode TEXT)"
                ))
            {
                qFatal("Failed to query database: %s", qPrintable(m_query.lastQuery()));
            }
        }
    }

    void OSMTableModel::clear()
    {
        beginResetModel();
        m_filter = "postcode = -1";
        endResetModel();
    }

    QHash<int, QByteArray> TownModel::roleNames() const
    {
        QHash<int, QByteArray> names;
        names[Qt::UserRole] = "id";
        names[Qt::UserRole + 1] = "city";
        names[Qt::UserRole + 2] = "postcode";
        return names;
    }

    QVariant TownModel::data(const QModelIndex &index, int role) const
    {
        if (role < Qt::UserRole)
            return QSqlTableModel::data(index, role);

        const QSqlRecord sqlRecord = record(index.row());
        return sqlRecord.value(role - Qt::UserRole);
    }

    void TownModel::insert(const QMap<QString, QString>&& map)
    {
        QSqlQuery m_query;

        QString city = map["city"];
        QString postcode = map["postcode"];

        if(!city.isEmpty() && !postcode.isEmpty())
        {
            m_query.prepare("SELECT * FROM Town WHERE postcode LIKE :postcode;");
            m_query.bindValue(":postcode", map["postcode"]);
            m_query.exec();
            if(!m_query.next())
            {
                //qDebug() << "Select * From Town WHERE postcode LIKE :" << postcode;
                m_query.prepare("INSERT INTO Town(city, postcode) VALUES (:city, :postcode)");
                m_query.bindValue(":city", city);
                m_query.bindValue(":postcode", postcode);
                if(!m_query.exec())
                {
                    qFatal("Couldnt insert Town %s", postcode.toStdString().c_str());
                    qFatal() << m_query.lastError();
                }
            }
        }
    }


    const QString StreetModel::tableName()
    {
        setTable("Street");
        return "Street";
    }

    StreetModel& StreetModel::operator=(const StreetModel& streetModel)
    {
        if (this == &streetModel)
            return *this;
        return *this;
    }

    void StreetModel::createTable()
    {
        if(tableNotExists())
        {
            QSqlQuery m_query;
            if(!m_query.exec(
                "CREATE TABLE IF NOT EXISTS Street ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "street TEXT,"
                "postcode TEXT,"
                "FOREIGN KEY(postcode) REFERENCES Town(postcode) ON DELETE CASCADE)"
                ))
            {
                qFatal("Failed to query database: %s", qPrintable(m_query.lastQuery()));
            }
        }
    }

    QHash<int, QByteArray> StreetModel::roleNames() const
    {
        QHash<int, QByteArray> names;
        names[Qt::UserRole] = "id";
        names[Qt::UserRole + 1] = "street";
        names[Qt::UserRole + 2] = "postcode";
        return names;
    }

    QVariant StreetModel::data(const QModelIndex &index, int role) const
    {
        if (role < Qt::UserRole)
            return QSqlTableModel::data(index, role);

        const QSqlRecord sqlRecord = record(index.row());
        return sqlRecord.value(role - Qt::UserRole);
    }

    void StreetModel::insert(const QMap<QString, QString>&& map)
    {
        QString street = map["street"];
        QString postcode = map["postcode"];

        QSqlQuery m_query;

        if(!street.isEmpty() && !postcode.isEmpty())
        {
            m_query.prepare("SELECT * FROM Street WHERE street LIKE :street AND postcode LIKE :postcode");
            m_query.bindValue(":street", street);
            m_query.bindValue(":postcode", postcode);
            m_query.exec();
            if(!m_query.next())
            {
                //qDebug() << "Select * From Street WHERE street LIKE " << street << " AND postcode LIKE " << postcode;
                m_query.prepare("INSERT INTO Street(street, postcode) VALUES (:street, :postcode)");
                m_query.bindValue(":street", street);
                m_query.bindValue(":postcode", postcode);
                if(!m_query.exec())
                {
                    qFatal("Couldnt insert Street %s %s",
                           map["street"].toStdString().c_str(),
                           m_query.lastError().text().toStdString().c_str()
                           );
                }
            }
        }
    }

    HouseNumberModel& HouseNumberModel::operator=(const HouseNumberModel& houseNumberModels)
    {
        if (this == &houseNumberModels)
            return *this;
        return *this;
    }

    const QString HouseNumberModel::tableName()
    {
        setTable("HouseNumber");
        return "HouseNumber";
    }

    void HouseNumberModel::createTable()
    {
        if(tableNotExists())
        {
            QSqlQuery m_query;
            if(!m_query.exec(
                "CREATE TABLE IF NOT EXISTS HouseNumber ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "postcode TEXT"
                "number TEXT,"
                "street TEXT,"
                "lon REAL,"
                "lat REAL,"
                "FOREIGN KEY(street) REFERENCES Street(street) ON DELETE CASCADE)"
                ))
            {
                qFatal("Failed to query database: %s",
                       qPrintable(m_query.lastQuery()));
            }
        }
    }


    QHash<int, QByteArray> HouseNumberModel::roleNames() const
    {
        QHash<int, QByteArray> names;
        names[Qt::UserRole] = "id";
        names[Qt::UserRole + 1] = "number";
        names[Qt::UserRole + 2] = "street";
        return names;
    }

    QVariant HouseNumberModel::data(const QModelIndex &index, int role) const
    {
        if (role < Qt::UserRole)
            return QSqlTableModel::data(index, role);

        const QSqlRecord sqlRecord = record(index.row());
        return sqlRecord.value(role - Qt::UserRole);
    }

    void HouseNumberModel::insert(const QVariantMap&& map)
    {
        QString postcode = map.value("postcode").toString();
        QString street = map.value("street").toString();
        QString number = map.value("number").toString();
        qreal lat = map.value("lat").toDouble();
        qreal lon = map.value("lon").toDouble();
        QSqlQuery m_query;

        if(!street.isEmpty() && !number.isEmpty())
        {
            m_query.prepare("SELECT * FROM HouseNumber WHERE postcode = :postcode AND street = :street AND number LIKE :number");
            m_query.bindValue(":postcode", postcode);
            m_query.bindValue(":street", street);
            m_query.bindValue(":number", number);
            m_query.exec();
            if(!m_query.next())
            {
                //qDebug() << "Select * From HouseNumber WHERE street LIKE " << street << " AND number LIKE " << number;
                m_query.prepare("INSERT INTO HouseNumber(postcode, number, street, lon, lat) VALUES (:number, :street, :lon, :lat)");
                m_query.bindValue(":postcode", postcode);
                m_query.bindValue(":street", street);
                m_query.bindValue(":number", number);
                m_query.bindValue(":lon", lon);
                m_query.bindValue(":lat", lat);
                if(!m_query.exec())
                {
                    qFatal("Couldnt insert Street: %s with Number:",
                           m_query.lastError().text().toStdString().c_str()
                           );
                }
            }
        }
    }

    QVariantList HouseNumberModel::queryHouseNumberLocations(const QVariantList& queryList)
    {
        QSqlQuery m_query;
        QVariantList results;

        int args = 0;

        // Generate placeholders for each value
        for (int i = 0; i < queryList.size(); ++i) {

            QVariantMap stops = queryList[i].toMap();
            qDebug() << stops;
            QString queryString = QString("SELECT lon, lat FROM houseNumber "
                                          "WHERE postcode = '%1' "
                                          "AND street = '%2' "
                                          "AND number IN (").arg(stops["postalCode"].toString(),
                                           stops["street"].toString());

            QStringList houseNumberList = stops["numbers"].toStringList();


            for(int h = 0; h < houseNumberList.size(); h++)
            {
                queryString.append("'");
                queryString.append(houseNumberList[h]);
                queryString.append("'");
                if(h < houseNumberList.size() - 1) {
                    queryString.append(", ");
                }
            }

            queryString.append(")");

            qDebug() << queryString;

            m_query.prepare(queryString);
            // Execute the query
            if (m_query.exec()) {
                if (results.isEmpty()) {
                    qWarning() << "No results found for the query.";
                }

                while (m_query.next()) {
                    QVariantMap result;
                    result["lon"] = m_query.value(0);
                    result["lat"] = m_query.value(1);
                    results.append(result);
                }
            } else {
                qWarning() << "Query failed:" << m_query.lastError().text();
            }
        }
        return results;
    }

}
