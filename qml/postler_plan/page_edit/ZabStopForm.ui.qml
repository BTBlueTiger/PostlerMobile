
import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls 2.15

Grid {

    property alias stateIcon : toolBtnState.icon
    property alias houseNumber : houseNumberTextField.text
    property alias isPackage : toolBtnPackage
    property alias isLetter : toolBtnMail
    property alias deleteNumber : toolBtnDelete
    property alias addRow : btnNewRow

    rows: 2
    Row {
        Rectangle {
            id: rectangle
            color: "gray"
            height: 100
            width: grid.width
            border.color: Material.primary
            border.width: 1
            radius: 5

            RowLayout {
                width: parent.width
                id: componetRow
                anchors.verticalCenter: parent.verticalCenter
                Layout.margins: 10

                Text {
                    Layout.margins: 10
                    text: index < 10 ? "0" + (index + 1) : (index + 1)
                }

                TextField {
                    id: houseNumberTextField
                    placeholderText: "Hnr."
                    text: model.number
                    horizontalAlignment: Text.AlignHCenter
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    placeholderTextColor: "black"
                }

                RowLayout {
                    ToolButton {
                        id: toolBtnPackage
                        icon.source: "qrc:/icons/icons/package.svg"
                        icon.color: model.package ? Material.primary : "white"
                    }
                    ToolButton {
                        id: toolBtnMail
                        icon.source: "qrc:/icons/icons/mail.svg"
                        icon.color: model.letter ? Material.primary : "white"
                    }
                    ToolButton {
                        id: toolBtnDelete
                        icon.source: "qrc:/icons/icons/delete.svg"
                        icon.color: "red"
                    }
                    ToolButton {
                        id: toolBtnState
                    }
                }
            }
        }
    }
    Row {
        height: 25
        width: parent.width
        Button {
            id: btnNewRow
            height: parent.height
            width: parent.width
            text: "+"
        }
    }
}
