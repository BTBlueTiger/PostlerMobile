import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls 6.2

import "../custom/control"

import "page_edit"
import "page_load"

Item {

    property alias postlerPlanStackView : postlerPlanStackView

    property alias postlerPlanTabBar: postlerPlanTabBar

    property alias tabEdit : postlerPlanTabEdit
    property alias postlerPlanEditComponent  : postlerPlanEditComponent

    property alias tabLoad : postlerPlanTabLoad
    property alias postlerPlanLoadComponent : postlerPlanComponentConvert

    property alias tabSave : postlerPlanTabSave


    StackView {
        id: postlerPlanStackView
        initialItem: postlerPlanComponentConvert
        width: parent.width
        height: parent.height - pageTabBar.height
    }


    Component {
        id: postlerPlanComponentConvert
        PostlerPlanLoad {}
    }

    Component {
        id: postlerPlanEditComponent
        PostlerEditPage {}
    }



    Rectangle {
        id: bottomRect
        x: 0
        height: 72
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.topMargin: 0

        TabBar {
            id: postlerPlanTabBar
            height: parent.height
            width: parent.width

            TabButton {
                id: postlerPlanTabLoad
                text: qsTr("Konvertieren")
            }

            TabButton {
                id: postlerPlanTabEdit
                text: qsTr("Edititieren")
            }
            TabButton {
                id: postlerPlanTabSave
                text: qsTr("Speichern")
            }
        }
    }
}
