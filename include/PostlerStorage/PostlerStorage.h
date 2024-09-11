#ifndef POSTLERSTORAGE_H
#define POSTLERSTORAGE_H

#include "qvariant.h"
#include <qstring.h>

namespace PostlerStorage {

    enum SQL_T {
        TEXT,
        INTEGER,
        REAL,
        BOOL,
        DATETIME
    };

    enum QUERY_TYPE{
        SELECT,
        INSERT,
        UPDATE
    };

    static QString sql_T_ToString(const SQL_T type)
    {
        switch (type) {
        case TEXT:
            return "TEXT";
        case INTEGER:
            return "INTEGER";
        case REAL:
            return "REAL";
        case BOOL:
            return "INTGER";
        case DATETIME:
            return "DATETIME";
        default:
            break;
        }
        return "";
    }


    namespace TABLENAME {
        constexpr const char* HOUSE_NUMBER = "HOUSE_NUMBER";
        constexpr const char* ZAB = "ZAB";
        constexpr const char* ROUTE = "ROUTE";
        constexpr const char* STOP = "STOP";
    }

    namespace TABLE_FILTER {
        constexpr const char* VALUE = "val";
        constexpr const char* FILTER_T = "filter_t";
        constexpr const char* TABLE_ROLE = "table_role";
    }

    namespace SQL_VALUES {
        constexpr const char* ID = "id";
        constexpr const char* POSTAL_CODE = "postal_code";
        constexpr const char* STREET = "street";
        constexpr const char* NUMBER = "number";

    namespace STOP {
        constexpr const char* ID  = SQL_VALUES::ID;
        constexpr const char* ZAB = "_zab";
        constexpr const char* ROUTE = "_route";
        constexpr const char* HOUSE_NUMBER  = "_house_number";
        constexpr const char* PACKAGE = "package";
        constexpr const char* LETTER = "letter";
        constexpr const char* GOODS = "goods";
        constexpr const char* MAIL_BOX = "mail_box";
        constexpr const char* PICKUP = "pickup";
        constexpr const char* VERIFY = "verify";
    }

    namespace HOUSE_NUMBER {
        constexpr const char* ID  = SQL_VALUES::ID;
        constexpr const char* POSTAL_CODE = SQL_VALUES::POSTAL_CODE;
        constexpr const char* STREET = SQL_VALUES::STREET;
        constexpr const char* NUMBER = SQL_VALUES::NUMBER;
        constexpr const char* STATE_QUERY = "queryState";
        constexpr const char* LAT = "lat";
        constexpr const char* LON = "lon";
    }

    namespace ZAB {
        constexpr const char* ID  = SQL_VALUES::ID;
        constexpr const char* NUMBER = "number";
        constexpr const char* POSTAL_CODE = SQL_VALUES::POSTAL_CODE;
        constexpr const char* STREET = SQL_VALUES::STREET;
        constexpr const char* PART = "part";
        constexpr const char* ROUTE = "_route";
    }

    namespace ROUTE {
        constexpr const char* NAME = "name";
        constexpr const char* TIMESTAMP = "time_stamp";
    }

    }
}
#endif // POSTLERSTORAGE_H
