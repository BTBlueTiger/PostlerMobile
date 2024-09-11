import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    property alias text : textToDelete.text
    property alias btnDelete : btnDelete
    color: "transparent"
    radius: 2
    border.color: "lightgray"
    RowLayout {
        width: parent.width
        height: parent.height
        Text {
            id: textToDelete
            color: Material.primary
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            font.pointSize: 24
            font.bold: true
            Layout.leftMargin: 20
        }

        Button {
            id: btnDelete
            icon.source: "qrc:/content/icons/delete.svg"
            Layout.rightMargin: 20
        }
    }
}
