import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtLocation
import QtPositioning

import QtSensors
import GeoPositionRessource

Map {


    property alias btnFollow : btnFollow
    property alias btnNext : btnNext
    property alias btnBefore : btnBefore
    property alias btnNextZab : btnNextZab
    property alias btnBeforeZab : btnBeforeZab

    property alias firstMarker : firstMarker
    property alias secondMarker : secondMarker

    property alias pinchHandler : pinchHandler
    property alias dragHandler : dragHandler
    property alias wheelHandler : wheelHandler

    property alias navigationMapitem : navigationMapitem


    plugin: osmPlugin

    zoomLevel: 14
    activeMapType: supportedMapTypes[supportedMapTypes.length - 1]




    ColumnLayout {

        // Remove explicit width and height to let the layout manage size
        // width: curStopText.implicitWidth
        // height: curStopText.implicitHeight * 2

        anchors.right: parent.right  // Anchors to the right side of the parent
        anchors.rightMargin: 10      // Add margin if needed

        RowLayout{
            property var curStop : routingManager.cur_stop
            Layout.fillWidth: true
            Image {
                id: curStopPackage
                visible: routingManager.cur_stop.package
                source: "qrc:/icons/icons/package.svg"
            }
            Image {
                id: curStopLetter
                visible: routingManager.cur_stop.letter
                source: "qrc:/icons/icons/mail.svg"
            }
            TextField {
                id: curStopText
                text: routingManager.cur_stop.street + " " + routingManager.cur_stop.number
                font.pixelSize: 18
                background: Rectangle {
                    color: "black"
                }
                Layout.fillWidth: true  // Allow TextField to take available width
            }
        }


        RowLayout{
            Layout.fillWidth: true
            Image {
                id: nextStopPackage
                visible: routingManager.next_stop.package
                source: "qrc:/icons/icons/package.svg"
            }
            Image {
                id: nextStopLetter
                visible: routingManager.next_stop.letter
                source: "qrc:/icons/icons/mail.svg"
            }
            TextField {
                id: nextStopText
                text: routingManager.next_stop.street + " " + routingManager.next_stop.number
                font.pixelSize: 18
                background: Rectangle {
                    color: "black"
                }

                Layout.fillWidth: true  // Allow TextField to take available width
            }
        }
    }

    ToolButton{
        id: btnFollow
        icon.source : shouldFollow ? "qrc:/icons/icons/explore.svg" : "qrc:/icons/icons/explore_off.svg"
        anchors {
            left: parent.left
            top: parent.top
        }
        icon.color: Material.primary
    }

    RowLayout {
        anchors{
            bottom: parent.bottom
            right: parent.right
        }
        Layout.fillWidth: true

        ColumnLayout {

            Button {
                id: btnFilter
                text: "Filter";
            }

        }

        ColumnLayout {
            Text {
                text: "Back"
            }
            Button {
                id: btnBeforeZab
                text: "Zab";
            }
            Button {
                id: btnBefore
                text: "Stop";
            }
        }

        ColumnLayout {
            Text {
                text: "Next"
            }
            Button {
                id: btnNextZab
                text: "Zab";
            }
            Button {
                id: btnNext
                text: "Stop";
            }
        }
    }

    ColumnLayout{
        anchors{
            bottom: parent.bottom
            right: parent.right
        }




    }


    MapItemView {
        id: mapRoute
        model: routeModel
        delegate: MapRoute {
            route: routeData
            line.color: "blue"
            line.width: 5
            smooth: true
        }
    }

    MapQuickItem {
        id: navigationMapitem
        visible: shouldFollow ? true: false
        anchorPoint.x: icon.width / 2
        anchorPoint.y: icon.height
        sourceItem: Image{
            id: icon
            source: "qrc:/icons/icons/navigation.svg"
        }
        coordinate: center
    }

    // Simply repeating "marked" locations
    /*
MarkerRepeater {
        id: markerRepeater
        model: waypoints
    }
    */

    MapQuickItem {
            id: firstMarker
            anchorPoint.x: icon.width / 2
            anchorPoint.y: icon.height
            sourceItem: Text {
                text: "1"
                    Image {
                    source: "qrc:/icons/icons/drop.svg"  // Path to your marker icon
                    width: 40
                    height: 40
                }
            }
        }
    MapQuickItem {
            id: secondMarker
            anchorPoint.x: icon.width / 2
            anchorPoint.y: icon.height
            sourceItem: Text {
                text: "2"
                    Image {
                    source: "qrc:/icons/icons/drop.svg"  // Path to your marker icon
                    width: 40
                    height: 40
                }
            }
        }

    PinchHandler {
        id: pinchHandler
        target: null
        grabPermissions: PointerHandler.TakeOverForbidden
    }

    WheelHandler {
        id: wheelHandler
        // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
        // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
        // and we don't yet distinguish mice and trackpads on Wayland either
        acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                         ? PointerDevice.Mouse | PointerDevice.TouchPad
                         : PointerDevice.Mouse
        rotationScale: 1/120
        property: "zoomLevel"
    }

    DragHandler {
        id: dragHandler
        target: null
    }

    Shortcut {
        enabled: zoomLevel < maximumZoomLevel
        sequence: StandardKey.ZoomIn
        onActivated: zoomLevel = Math.round(zoomLevel + 1)
    }
    Shortcut {
        enabled: zoomLevel > minimumZoomLevel
        sequence: StandardKey.ZoomOut
        onActivated: zoomLevel = Math.round(zoomLevel - 1)
    }


}
