

import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls 2.15

import PostlerStorage
import ZabTable

Rectangle {

    property alias zabListView: zabListView

    color: "darkGrey"

    Rectangle {
        id: rectListView
        width: parent.width
        height: parent.height

        ListView {
            id: zabListView
            width: parent.width
            height: parent.height
            model: ZabTable
            spacing: 5
        }
    }
}

