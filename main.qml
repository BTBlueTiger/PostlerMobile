import QtQuick 2.15
import ScreenResolution



import "qml"

Window {
    property var screenResultion : ScreenResolution {}

    /*
    Component.onCompleted: {
        PostlerDataBase.open()
        RoutingManagerTableModel.createTable()
        ZabTableModel.createTable()
        HouseNumberTableModel.createTable()
    }
    */

    width: 1080 / 2
    height: 2400 / 2

    visible: true
    title: "Postler"

    MainFrame {
        id: mainFrame
        width: parent.width
        height: parent.height
    }


}

