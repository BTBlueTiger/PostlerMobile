#ifndef NOMINATIMRESSOURCE_H
#define NOMINATIMRESSOURCE_H

#include <QObject>
#include <QJsonArray>

#include <QQmlEngine>
#include <QJSEngine>

#include "../../RequestReceiver.h"

#include <QtNetwork/qrestaccessmanager.h>
#include <QtNetwork/qnetworkrequestfactory.h>
#include <QtNetwork/qrestaccessmanager.h>
#include <QtNetwork/qnetworkrequestfactory.h>
#include <QtNetwork/qhttpheaders.h>
#include <QtNetwork/qrestaccessmanager.h>
#include <QtNetwork/qrestreply.h>

#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>

#include <QMap>

class NominatimRessource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList search WRITE setSearch FINAL)
    Q_PROPERTY(bool running NOTIFY runningChanged FINAL)
    Q_PROPERTY(QVariant result READ result NOTIFY resultChanged FINAL)
    Q_PROPERTY(int resultSize READ resultSize NOTIFY resultSizeChanged FINAL)

public:
    explicit NominatimRessource(QObject *parent = nullptr);
    static QObject* createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);
    void setSearch(const QVariantList& search);
    void setSearch(const QString& queryString, RequestReceiver* receiver);
    QNetworkReply* createQNetworkReply(const QString& queryString);
    void setResultSize(int);
    int resultSize();
    QNetworkReply* getRequest(const QNetworkRequest& request);
    QVariant result() const;
    bool running();
    void setRunning(bool running);

signals:
    void success(const QJsonArray& data = QJsonArray());
    void error(const QString&);
    void runningChanged(const bool&);
    void resultSizeChanged();
    void resultChanged();

private slots:
    void networkRequestFinished(QNetworkReply*, int);
    void decrementQueryCounter();

private:
    QVariantList m_search;
    QNetworkAccessManager m_manager;
    QNetworkRequestFactory m_api;
    QUrl url;
    QMap<int, QString> m_short_display_name;
    QMap<int, bool> m_package;
    static NominatimRessource *m_instance;

    QVariantMap m_result;
    int m_query_index = 0, m_resultSize;
    bool m_running = false;



signals:
};

#endif // NOMINATIMRESSOURCE_H
