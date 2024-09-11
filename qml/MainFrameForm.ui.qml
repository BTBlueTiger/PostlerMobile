

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts

import QtPositioning

import "postler_map"
import "postler_plan"
import "postler_storage"

Rectangle {
    id: rectangle

    property var mapCenter

    property alias pageTabBtnStorage: pageTabBtnStorage
    property alias pageTabBtnPlan: pageTabBtnPlan
    property alias pageTabBtnMap: pageTabBtnMap

    property alias pageTabBar: pageTabBar

    property alias stackView: stackView

    property alias postlerMapComponent: postlerMapComponent
    property alias postlerPlanComponent: postlerPlanComponent
    property alias postlerStorageComponent: postlerStorageComponent


    TabBar {
        id: pageTabBar
        x: 0
        y: 0
        width: parent.width
        currentIndex: 1

        TabButton {
            id: pageTabBtnMap
            text: qsTr("Map")
            icon.source: "qrc:/icons/icons/map.svg"
        }
        TabButton {
            id: pageTabBtnPlan
            text: qsTr("Plan")
            icon.source: "qrc:/icons/icons/target.svg"
        }
        TabButton {
            id: pageTabBtnStorage
            text: qsTr("Storage")
            icon.source: "qrc:/icons/icons/storage.svg"
        }
    }

    StackView {
        id: stackView
        initialItem: postlerPlanComponent
        width: parent.width
        height: parent.height - pageTabBar.height
        y: pageTabBar.height
    }

    Component {
        id: postlerMapComponent

        PostlerMap {
            id: postlerMap
        }
    }

    Component {
        id: postlerPlanComponent
        PostlerPlan {
            id: postlerPlan
        }
    }

    Component {
        id: postlerStorageComponent
        PostlerStorage {
            id: postlerStorage
        }
    }
}
