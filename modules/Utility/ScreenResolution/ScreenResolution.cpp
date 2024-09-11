#include "ScreenResolution.h"
#include "QtGui/qscreen.h"

ScreenResolution::ScreenResolution(QObject *parent) : QObject(parent)
{
    QList<QScreen *> screens = QGuiApplication::screens();
    if(!screens.isEmpty())
    {
        QScreen * screen = screens.first();
        m_size = screen->size();
    }
}

void ScreenResolution::update()
{
    QList<QScreen *> screens = QGuiApplication::screens();
    if(!screens.isEmpty())
    {
        QScreen * screen = screens.first();
        m_size = screen->size();
        emit changed(m_size.width(), m_size.height());
    }
}

QSizeF ScreenResolution::get()
{
    return m_size;
}
