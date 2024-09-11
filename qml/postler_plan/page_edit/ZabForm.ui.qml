
import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls 2.15

import PostlerStorage

Rectangle {

    property alias headerGrid : grid
    property alias houseNumberLoader : houseNumberLoader

    property alias stopsRect : stopRect

    property alias zabNumber: textEditZab.text
    property alias streetName: textEditStreet.text
    property alias btnUp: roundBtnUp
    property alias btnDown: roundBtnDown
    property alias btnDelete: roundBtnDelete
    property alias btnUnfold: roundButtonUnfold

    property alias listViewComponent : listViewComponent

    color: "#4b4a4a"

    Grid {
        id: grid
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: gridRow.implicitHeight
        //anchors.bottom: isHouseNumbersOpen ? parent.verticalCenter : parent.bottom
        anchors.margins: 20
        rows: 1

        Row {
            id: gridRow
            //anchors.verticalCenter: parent.verticalCenter
            Layout.fillWidth: true
            width: parent.width
            spacing: 10

            Column {
                width: parent.width * (1 / 10)
                TextEdit {
                    id: textEditZab
                    text: qsTr("Zab:") + zabNumber
                    font.pixelSize: 24
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    color: Material.primary
                }
            }

            Column {
                width: parent.width * (6 / 10)

                TextField {
                    width: parent.width
                    placeholderText: "Straße"
                    id: textEditStreet
                    font.pixelSize: 24
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    color: Material.primary
                }
            }

            Column {

                width: parent.width * (2 / 10)
                Row {
                    RoundButton {
                        id: roundBtnUp
                        icon.source: "qrc:/icons/icons/arrow_circle_up.svg"
                    }

                    RoundButton {
                        id: roundBtnDelete
                        icon.source: "qrc:/icons/icons/delete.svg"
                    }
                }
                Row {

                    RoundButton {
                        id: roundBtnDown
                        icon.source: "qrc:/icons/icons/arrow_circle_down.svg"
                    }

                    RoundButton {
                        id: roundButtonUnfold
                        icon.source: isUnfold ? "qrc:/icons/icons/unfold_less.svg" : "qrc:/icons/icons/unfold_more.svg"
                    }
                }
            }
        }
    }

    Rectangle {
        id: stopRect
        width: parent.width
        height: parent.height - grid.height
        anchors.top: grid.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        color: parent.color
        visible: isUnfold
        Loader {
            id: houseNumberLoader
            sourceComponent: isUnfold ? listViewComponent : null

            width: parent.width
            height: parent.height

            Component {
                id: listViewComponent
                /*
                ScrollView {

                    width: parent.width
                    height: parent.height
                    contentHeight: houseNumberFlow.implicitHeight
                    contentWidth: houseNumberFlow.implicitWidth
*/
                    Flow {
                        id: houseNumberFlow
                        padding: 5
                        spacing: 5
                        width: parent.width
                        height: parent.height

                        Repeater {
                            model: stopModel
                            delegate: ZabStop {}
                        }
                    }
                }

           // }

        }
    }
}


