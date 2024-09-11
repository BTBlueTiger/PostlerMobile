#include "NominatimRessource.h"

#include "../../Constants.h"

NominatimRessource *NominatimRessource::m_instance = nullptr;

int NominatimRessource::resultSize()
{
    return m_resultSize;
}

void NominatimRessource::setResultSize(int)
{
    m_resultSize = resultSize();
}


QVariant NominatimRessource::result() const
{
    return m_result;
}

bool NominatimRessource::running()
{
    return m_running;
}

void NominatimRessource::setRunning(bool running)
{
    if(running != m_running)
    {
        m_running = running;
        emit runningChanged(running);
    }
}

NominatimRessource::NominatimRessource(QObject *parent)
    : QObject{parent}
{
    url.setScheme("http");
    url.setHost(NOMINATIM_HOST);
    url.setPort(NOMINATIM_PORT);
    m_api.setBaseUrl(url);
    m_search.clear();
    m_result = QVariantMap();
    connect(this, &NominatimRessource::success, &NominatimRessource::decrementQueryCounter);
    connect(this, &NominatimRessource::error, &NominatimRessource::decrementQueryCounter);
}

QObject* NominatimRessource::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine){
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(m_instance==nullptr) { m_instance = new NominatimRessource;}
    return m_instance;
}

void NominatimRessource::setSearch(const QString& queryString, RequestReceiver* receiver)
{
    QNetworkReply *reply = m_manager.get(m_api.createRequest(queryString));
    connect(reply, &QNetworkReply::finished, this, [receiver, reply]() {
        receiver->requestFinished(reply);
    });

}

QNetworkReply* NominatimRessource::createQNetworkReply(const QString& queryString)
{
    return m_manager.get(m_api.createRequest(queryString));
}

void NominatimRessource::setSearch(const QVariantList& search)
{
        m_result.clear();
        m_query_index = 0;
        setRunning(true);
        m_search = search;

        for (int i = 0; i < search.size(); ++i)
        {
            QVariantMap stops = search[i].toMap();
            //QString query = "search?q=";
            QString postalCode = stops["postalCode"].toString();
            QString street = stops["street"].toString();
            QString houseNumber = stops["number"].toString();
            QString query = QString("search?q=%1 %2 %3").arg(
                postalCode,
                street,
                houseNumber);
            m_short_display_name[i] = QString("%1 : %2").arg(street, houseNumber);
            m_package[i] = !stops["package"].toString().isEmpty();
            QNetworkReply *reply = m_manager.get(m_api.createRequest(query));
            connect(reply, &QNetworkReply::finished, this, [=]() {
                networkRequestFinished(reply, i);
            });
        }
}


void NominatimRessource::networkRequestFinished(QNetworkReply* reply, int index)
{
    if(reply->isFinished())
    {
        if(reply->error())
        {
            emit error(reply->errorString());
            qDebug() << reply->errorString();
            reply->deleteLater();
        }
        else
        {
            QByteArray responseData = reply->readAll();
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            const QJsonArray jsonArray = jsonDoc.array();
            if (jsonArray.isEmpty())
                emit error("Anwser is empty");
            else {
                QJsonValue firstObject = jsonArray.first();
                QMap objVar = firstObject.toVariant().toMap();
                objVar["short_display_name"] = m_short_display_name[index];
                objVar["package"] = m_package[index];
                m_result[QString::number(index)] = objVar;
                reply->deleteLater();
                emit success(jsonArray);
            }
        }
    }
}

void NominatimRessource::decrementQueryCounter()
{
    if(m_running)
    {
        m_query_index ++;
        if(m_query_index + 1 == m_search.size())
        {
            qDebug() << m_result;
            setRunning(false);
        }
    }
}
