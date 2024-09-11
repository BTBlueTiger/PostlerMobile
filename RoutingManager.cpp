#include "RoutingManager.h"
#include <QDebug>

void HousNumber::ceckStop(NominatimRessource* ressource)
{
    QVariantMap search;
    search["postalCode"] = "32584";
    search["street"] = street;
    search["number"] = number;
    //ressource->setSearch(search, this);
}

void HousNumber::requestFinished(QNetworkReply* reply)
{
    if(reply->isFinished())
    {
        if(reply->error() != QNetworkReply::NoError)
        {
            zabStopState = ERROR;
        }
        else
        {
            QByteArray responseData = reply->readAll();
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            const QJsonArray jsonArray = jsonDoc.array();
            if (jsonArray.isEmpty())
            {
                zabStopState = EMPTY;
            }
            else
            {
                if(jsonArray.size() > 1)
                {
                    zabStopState = VALID_MANY;
                }
                else
                {
                    zabStopState = VALID_ONE;
                }
                requestResult = jsonArray;
                QJsonValue firstObject = jsonArray.first();
                QMap objVar = firstObject.toVariant().toMap();
                lat = objVar["lat"].toDouble();
                lon = objVar["lon"].toDouble();
                zabModel->zabStopChanged(row);
            }
        }
        zabModel->incrementCheckedZabs();
    }
}

QVariant HousNumber::asQVariant()
{
    QVariantMap z;
    z["street"] = street;
    z["number"] = number;
    z["postal_code"] = postalCode;
    z["package"] = package;
    z["letter"] = letter;
    z["lat"] = lat;
    z["lon"] = lon;
    return z;
}

ZabModel::ZabModel(QObject *parent)
    : QAbstractListModel(parent) {
    //connect(this, SIGNAL(areHouseNumbersCheckedChanged()), m_routingManager, SLOT(m_routingManager->zabCheckedChanged(m_areZabsChecked)));
}

void ZabModel::incrementCheckedZabs()
{
    m_checked_zabs ++;
    m_areHouseNumbersChecked = m_checked_zabs == m_zabStops.size() ? true : false;
    emit areHouseNumbersCheckedChanged(m_areHouseNumbersChecked);

}

void ZabModel::zabStopChanged(int row)
{
    // Create QModelIndex for the row
    QModelIndex index = createIndex(row, 0);
    emit dataChanged(index, index, { roleNames().keys().toVector() } );
}

int ZabModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_zabStops.size();
}

QVariant ZabModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_zabStops.count()) {
        return QVariant();
    }

    if(role == StreetRole)
        return QVariant::fromValue(m_street);

    const HousNumber* zabStop = m_zabStops[index.row()];

    switch (role) {
    case NumberRole:
        return zabStop->number;
    case PackageRole:
        return zabStop->package;
    case LetterRole:
        return zabStop->letter;
    case LatRole:
        return zabStop->lat;
    case LonRole:
        return zabStop->lon;
    case ZabStopStateRole:
        return zabStop->zabStopState;
    default:
        return QVariant();
    }
}


bool ZabModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
        return false;
    HousNumber* zabStop = m_zabStops[index.row()];
    switch (role) {
    case NumberRole:
        zabStop->number = value.toString();
        break;
    case PackageRole:
        zabStop->package = value.toBool();
        break;
    case LetterRole:
        zabStop->letter = value.toBool();
        break;
    case LatRole:
        zabStop->lat = value.toDouble();
        break;
    case LonRole:
        zabStop->lon = value.toDouble();
        break;
    case ZabStopStateRole:
        zabStop->zabStopState = value.toInt();
        break;
    default:
        return false;
    emit dataChanged(index, index, { role } );
    }
    return true ;
}

QHash<int, QByteArray> ZabModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[StreetRole] = "street";
    roles[NumberRole] = "number";
    roles[PackageRole] = "package";
    roles[LetterRole] = "letter";
    roles[LatRole] = "lat";
    roles[LonRole] = "lon";
    roles[ZabStopStateRole] = "zabStopState";
    return roles;
}

void ZabModel::addZabStop(HousNumber *zabStop) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    zabStop->zabModel = this;
    zabStop->street = m_street;
    zabStop->row = rowCount();
    zabStop->zabStopState = HousNumber::ZabStopStates::DEFAULT;
    m_zabStops.append(zabStop);
    endInsertRows();
}

void ZabModel::addZabStop(int index) {
    beginInsertRows(QModelIndex(), index, index);

    /*
    HousNumber* zabStop = new HousNumber(this);
    zabStop->zabModel = this;
    zabStop->street = m_street;
    zabStop->postalCode = m_postalCode;
    zabStop->row = index;
    zabStop->zabStopState = HousNumber::ZabStopStates::DEFAULT;
    updateRowIndices(index + 1);
    m_zabStops.append(zabStop);*/
    endInsertRows();
}

void ZabModel::removeZabStop(int index) {
    if (index < 0 || index >= m_zabStops.size())
        return;
    beginRemoveRows(QModelIndex(), index, index);
    m_zabStops.removeAt(index);
    updateRowIndices(index);
    endRemoveRows();
}

void ZabModel::updateRowIndices(int startIndex)
{
    for (int i = startIndex; i < m_zabStops.size(); ++i)
        m_zabStops[i]->row = i;

}

void ZabModel::checkStops(NominatimRessource* res) {
    std::for_each(m_zabStops.begin(), m_zabStops.end(), [&res](HousNumber* z){
        z->ceckStop(res);
    });
}

QVector<HousNumber*> ZabModel::houseNumbers()
{
    return m_zabStops;
}

QString ZabModel::street() const
{
    return m_street;
}

QString ZabModel::postalCode() const
{
    return m_postalCode;
}

RoutingManager::RoutingManager(QObject* parent)
    : QAbstractListModel(parent), m_index(0) {
    connect(this, &RoutingManager::indexChanged, &RoutingManager::notifyIndexChanged);
}

QVariant RoutingManager::route() const {
    return m_route;
}

QVariant RoutingManager::getStop(int index) const
{
    ZabModel* m = m_zabs[m_currentZab];
    // Next Zab
    if(index < 0 && m_currentZab > 0)
    {
        m = m_zabs[m_currentZab -1];
        index = m->rowCount();
    }
    else if(m->rowCount() <= index && m_currentZab <= m_zabs.size())
    {
        m = m_zabs[m_currentZab + 1];
        index = 0;
    }
    HousNumber * z = m->getZabStop(index);
    if(z)
        return z->asQVariant();
    return QVariant();
}

QVariant RoutingManager::cur_stop() const {
    return getStop(m_index);
}

QVariant RoutingManager::next_stop() const {
    return getStop(m_index + 1);
}

QVariant RoutingManager::prev_stop() const {
    return getStop(m_index - 1);
}


int RoutingManager::index() const {
    return m_index;
}

void RoutingManager::setRoute(const QVariant& newRoute) {
    if (m_route != newRoute) {
        m_route = newRoute;
        isNewRoute = true;
        setIndex(0);
        emit routeChanged();
    }
}

void RoutingManager::setIndex(int newIndex) {

    m_index = newIndex;

    emit indexChanged();
    isNewRoute = false;
}

void RoutingManager::notifyIndexChanged()
{
    emit next_stopChanged();
    emit prev_stopChanged();
    emit cur_stopChanged();
}

void RoutingManager::next() {
    qDebug() << m_zabs[m_currentZab]->rowCount();
    qDebug() << m_index;
    if(m_zabs[m_currentZab]->rowCount() >= m_index + 1)
        setIndex(m_index + 1);
    else if(m_zabs.size() >= m_currentZab + 1)
    {
        m_currentZab += 1;
        setIndex(0);
    }
}

void RoutingManager::back() {
    if(m_index - 1 >= 0) {
        setIndex(m_index - 1);
    }
    else if(m_currentZab > 0 && m_index <= 0)
    {
        m_currentZab -= 1;
        setIndex(m_zabs[m_currentZab]->rowCount() - 1);
    }
}

void RoutingManager::removeZab(int index)
{
    if (index < 0 || index >= m_zabs.size())
        return;
    beginRemoveRows(QModelIndex(), index, index);
    m_zabs.removeAt(index);
    endRemoveRows();
}

void ZabModel::setStreet(const QString& s)
{
    if(s != m_street)
    {
        m_street = s;
        emit streetChanged();
    }
}

void ZabModel::setPostalCode(const QString& p)
{
    if(p != m_postalCode)
    {
        m_postalCode = p;
        emit postalCodeChanged();
    }
}

void RoutingManager::setWayPoints(int amount)
{

}

void RoutingManager::textToZabs(const QVariant& list)
{
    QVariantList rawList = list.toList();
    m_zabs.clear();
    m_n_stops = 0;
    m_currentZab = 0;
    for(const auto& l : rawList)
    {

        QString line = l.toString().simplified();
        if(line.isEmpty())
            continue;
        int index = 0;
        while(index ++ < line.size())
            if(line[index].isDigit())
                break;
        QString street = line.sliced(0, index);
        QStringList numbers = line.sliced(index).split(",");
        ZabModel* newZab = new ZabModel(this);
        newZab->setRoutingManager(this);
        newZab->setPostalCode("32584");
        newZab->setStreet(street);
        m_n_stops += numbers.size();
        for(const auto& n: numbers)
        {
            //newZab->addZabStop(new HousNumber(n, this));
        }
        m_zabs.append(newZab);
    }
    emit nStopsChanged();
}

RoutingManager& RoutingManager::operator=(const RoutingManager& other) {
    if (this != &other) {  // Check for self-assignment
        // Copy non-pointer member variables
        m_waypoints = other.m_waypoints;
        m_n_stops = other.m_n_stops;
        m_currentZab = other.m_currentZab;
        m_checkedZabs = other.m_checkedZabs;
        m_allZabsChecked = other.m_allZabsChecked;
        m_route = other.m_route;
        m_index = other.m_index;
        isNewRoute = other.isNewRoute;

        // Deep copy of m_zabs (cloning each ZabModel*)
        qDeleteAll(m_zabs);  // Delete existing pointers in m_zabs to avoid memory leaks
        m_zabs.clear();  // Clear the current QVector
        m_zabs = other.zabs();
    }

    return *this;  // Return *this to allow chained assignment
}


int RoutingManager::rowCount(const QModelIndex &parent) const
{
    return m_zabs.size();
}

QVariant RoutingManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_zabs.count()) {
        return QVariant();
    }
    if(role == ZabRole)
    {
        ZabModel* m = m_zabs.at(index.row());
        QVariant qVariantModel = QVariant::fromValue(m);
        return qVariantModel;
    }
    return QVariant();
}


QHash<int, QByteArray> RoutingManager::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ZabRole] = "zab";
    return roles;
}

int RoutingManager::checkZabs(NominatimRessource* res)
{
    for(const auto& zab : m_zabs)
    {
        for(const auto& zabStop : zab->houseNumbers())
        {
            zabStop->ceckStop(res);
        }
    }
    return -1;
}

void RoutingManager::nextZab()
{
    if(m_currentZab < m_zabs.size())
    {
        m_currentZab +=1;
        setIndex(0);
    }
}

void ZabModel::setRoutingManager(RoutingManager* manager)
{
    m_routingManager = manager;
    connect(this, SIGNAL(areHouseNumbersCheckedChanged(bool)), m_routingManager, SLOT(zabCheckedChanged(bool)));
}

void RoutingManager::backZab()
{
    if(m_currentZab > 0)
    {
        m_currentZab -=1;
        setIndex(0);
    }
}

void RoutingManager::zabCheckedChanged(bool isChecked)
{
    isChecked ? m_checkedZabs ++ : m_checkedZabs --;
    if(m_checkedZabs == m_zabs.size())
    {
        m_allZabsChecked = true;
        emit allZabsCheckedChanged();
    }
    else
    {
        m_allZabsChecked = false;
        emit allZabsCheckedChanged();
    }
}

QVector<ZabModel*> RoutingManager::zabs() const
{
    return m_zabs;
}
