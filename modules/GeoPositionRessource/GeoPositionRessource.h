#ifndef GEOPOSITIONRESSOURCE_H
#define GEOPOSITIONRESSOURCE_H

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>
#include <QQmlEngine>
#include <QJSEngine>
#include <QNmeaPositionInfoSource>
#include <QSerialPort>
#include <QQueue>
#include "../../Constants.h"
#include <qfile.h>


class GeoPositionRessource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate coordinate MEMBER m_coordinate NOTIFY coordinateChanged FINAL)
    Q_PROPERTY(double direction MEMBER m_direction NOTIFY directionChanged FINAL)
    Q_PROPERTY(double groundSpeed MEMBER m_groundSpeed NOTIFY groundSpeedChanged FINAL)
    Q_PROPERTY(double avagareBearing MEMBER m_avarageBearing NOTIFY avagareBearingChanged FINAL)

public:
    GeoPositionRessource(QObject *parent = nullptr);

    static QObject* createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);
    Q_INVOKABLE void setMockResource();

private:
    static GeoPositionRessource *m_instance;
    QNmeaPositionInfoSource *nmeaSource;
    QGeoPositionInfoSource *m_source;
    QVector<QGeoPositionInfo> m_lastPositionInfosQueue;
    int m_queueSize = 2;
    QGeoCoordinate m_coordinate;
    double m_direction;
    double m_groundSpeed;
    double m_avarageBearing;

    QFile* logFile;
    QString logFilePath = QString("%1/%2").arg(ROOT_PATH, "geoposition_log.txt");
    void calculateBearing();

public slots:
    void positionUpdated(const QGeoPositionInfo& gpsPos);
    void positionError(QGeoPositionInfoSource::Error e){}
    void writeLogFile(const QGeoPositionInfo &gpsPos);
    void fillQueue(const QGeoPositionInfo& gpsPos);



signals:
    void coordinateChanged(const QGeoCoordinate& gpsPos);
    void movementDetected(const QGeoCoordinate& gpsPos);
    void directionChanged(const double& direction);
    void groundSpeedChanged(const double& direction);
    void avagareBearingChanged(const double& avagareBearing);
    void readyChanged();
    void useGpsChanged();

};

#endif // GEOPOSITIONRESSOURCE_H
