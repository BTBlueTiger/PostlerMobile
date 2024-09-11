#include "GeoPositionRessource.h"

GeoPositionRessource *GeoPositionRessource::m_instance = nullptr;

GeoPositionRessource::GeoPositionRessource(QObject* parent) : QObject(parent)
{
    m_source = QGeoPositionInfoSource::createDefaultSource(0);

    if (m_source)
    {
        m_source->setUpdateInterval(1000);
        m_source->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
        m_source->startUpdates();

        connect(m_source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
        connect(m_source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(fillQueue(QGeoPositionInfo)));

        //connect(m_source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(writeLogFile(QGeoPositionInfo)));
    }
}

void GeoPositionRessource::writeLogFile(const QGeoPositionInfo& gpsPos)
{
    /*
    if (logFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(logFile);

        // Corrected the placeholders to %1 and %2
        QString coord = QString("Coordinate=[%1, %2]\n")
                            .arg(gpsPos.coordinate().latitude())
                            .arg(gpsPos.coordinate().longitude());
        out << coord;

        // Corrected the placeholder to %1
        QString dir = QString("Direction=[%1]\n").arg(m_direction);
        out << dir;

        // Corrected the placeholder to %1
        QString speed = QString("Speed=[%1]\n").arg(m_groundSpeed);
        out << speed;

        logFile->close();

    } else {
        qWarning() << "Could not open log file for writing:" << logFile->errorString();
    }
*/
}

void GeoPositionRessource::setMockResource()
{
}

void GeoPositionRessource::calculateBearing()
{
    QGeoCoordinate first = m_lastPositionInfosQueue.first().coordinate();
    QGeoCoordinate last = m_lastPositionInfosQueue.last().coordinate();
    double lat1 = qDegreesToRadians(first.latitude());
    double lon1 = qDegreesToRadians(first.longitude());
    double lat2 = qDegreesToRadians(last.latitude());
    double lon2 = qDegreesToRadians(last.longitude());

    double dLon = lon2 - lon1;

    double x = sin(dLon) * cos(lat2);
    double y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
    double initialBearing = atan2(x, y);

    double initialBearingDegrees = qRadiansToDegrees(initialBearing);
    m_avarageBearing = fmod(initialBearingDegrees + 360.0, 360.0);
    emit avagareBearingChanged(m_avarageBearing);
}

void GeoPositionRessource::fillQueue(const QGeoPositionInfo& gpsPos)
{
    if(m_lastPositionInfosQueue.size() >= m_queueSize)
    {
        double distance = m_lastPositionInfosQueue.first().coordinate().distanceTo(gpsPos.coordinate());
        if(distance > 2)
        {
            m_lastPositionInfosQueue.removeFirst();
            m_lastPositionInfosQueue.append(gpsPos);
            emit movementDetected(gpsPos.coordinate());
        }
    } else if(m_lastPositionInfosQueue.size() < m_queueSize) {
        m_lastPositionInfosQueue.append(gpsPos);
    }
}

void GeoPositionRessource::positionUpdated(const QGeoPositionInfo& gpsPos)
{
    if(gpsPos.isValid())
    {
        m_coordinate = gpsPos.coordinate();
        emit coordinateChanged(gpsPos.coordinate());
        fillQueue(gpsPos);
        double direction = gpsPos.attribute(QGeoPositionInfo::Direction);
        double groundSpeed = gpsPos.attribute(QGeoPositionInfo::GroundSpeed);
    }

}

QObject* GeoPositionRessource::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine){
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(m_instance==nullptr) { m_instance = new GeoPositionRessource;}
    return m_instance;
}
