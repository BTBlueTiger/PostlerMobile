import QtQuick 2.15
import QtQuick.Controls
import QtLocation
import QtPositioning

MapItemView {
    Component.onCompleted: {
        for(var i = 0; i < waypoints.length; i++)
        {
            console.log(waypoints[i])
            console.log(waypoints[0][0])
        }
    }

    delegate: MapQuickItem {
        coordinate: QtPositioning.coordinate(lat, lon)
        anchorPoint.x: icon.width / 2
        anchorPoint.y: icon.height
        sourceItem: Text {
            id: name
            text: index
                Image {
                id: icon
                source: "qrc:/icons/icons/drop.svg"  // Path to your marker icon
                width: 40
                height: 40
            }
        }
    }
}
