#ifndef REQUESTRECEIVER_H
#define REQUESTRECEIVER_H

#include "qnetworkreply.h"



class RequestReceiver : public QObject
{
    Q_OBJECT
public:
    RequestReceiver(QObject* parent = nullptr) : QObject(parent) {};
public slots:
    virtual void requestFinished(QNetworkReply* reply) {}
};

#endif // REQUESTRECEIVER_H
