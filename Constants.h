#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <qglobal.h>

#define DATABASE_NAME "OSMDatabase.db"
#define POSTLER_STORAGE_PATH "PostlerStorage.db"

#ifdef Q_OS_ANDROID
#include "QStandardPaths"
#define ROOT_PATH QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
#else
#define ROOT_PATH "/home/kamalte/QProjects/Postler_Mobile/res"
#endif

#define NOMINATIM_HOST "192.168.178.23"
#define NOMINATIM_PORT 8080

#endif // CONSTANTS_H
