#ifndef SCREENRESULTION_H
#define SCREENRESULTION_H

#include "qsize.h"
#include <QObject>

class ScreenResolution : public QObject
{
    Q_OBJECT

public:
    explicit ScreenResolution(QObject *parent = nullptr);
    Q_INVOKABLE QSizeF get();

public slots:
    void update();

signals:
    void changed(qreal width, qreal height);

private:
    QSizeF m_size;
};


#endif // SCREENINFO_H
