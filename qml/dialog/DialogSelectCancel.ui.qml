import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Dialog {

    anchors.centerIn: parent

    width: 360
    height: 240

    id: dialogSelectCancel
    property alias descriptionText: descriptionText.text
    property alias btnSelect : btnSelect
    property alias btnCancel : btnCancel


    ColumnLayout{
        width: parent.width
        height: parent.height

        RowLayout {
            width: parent.width
            height: parent.height * .7
            Text {
                id: descriptionText
                text: "text"
                font.pointSize: 24
                color: Material.accent
            }

        }

        RowLayout {
            id: btnRow
            width: parent.width
            y: parent.height - btnRow.height

            Button {
                id: btnSelect
                text: "Select"
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            }

            // An empty item to push the cancel button to the right
            Item {
                Layout.fillWidth: true
            }

            Button {
                id: btnCancel
                text: "Cancel"
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }
        }
    }
}
