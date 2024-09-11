import QtQuick 6.2
import QtQuick.Controls

import GeoPositionRessource
import QtPositioning

PostlerMapForm {
    property bool shouldFollow : false

    width: parent.width
    height: parent.height

    property var lastCenter
    property var currentCenter

    // Property animations

    function calculateBearing() {
        var first = GeoPositionRessource.coordinate
        var route = routeModel.get(0); // Get the first route
        var firstSegment = route.segments[0]; // Get the first segment of the route
        var path = firstSegment.path; // Get the path from the first segment

        // Assuming you are trying to access the first element of the path
        var last = path[path.length - 1]; // Access the first element of path


        bearing = first.azimuthTo(last)
    }



    Connections {
        target: GeoPositionRessource
        function onMovementDetected() {
            updateRouteModel()

            if(shouldFollow)
            {
                calculateBearing()
                setCenter()
            }
        }

        function onCoordinateChanged(gpsPos) {

            /*if(shouldFollow)
                setCenter(gpsPos)
        */
        }

    }


    function setCenter() {
        lastCenter = currentCenter
        currentCenter = GeoPositionRessource.coordinate
        center = currentCenter
    }

    Behavior on center {
      CoordinateAnimation {
        duration: 400
        easing.type: Easing.InOutQuad
       }
    }


    btnFollow.onClicked: {
        shouldFollow = !shouldFollow
        if(shouldFollow)
        {
            setCenter();
        }
    }

    btnFollow.text: shouldFollow ? "Stop" : "Start"
    center: shouldFollow ? currentCenter : lastCenter


    btnNext.onClicked: next()
    btnBefore.onClicked: before()

    btnNextZab.onClicked: nextZab()
    btnBeforeZab.onClicked: beforeZab()



    pinchHandler.onScaleChanged: (delta) => {
        zoomLevel += Math.log2(delta)
    }

    dragHandler.onTranslationChanged: (delta) => pan(-delta.x, -delta.y)


    firstMarker.coordinate : stopToCoordinate(routingManager.cur_stop)
    secondMarker.coordinate: stopToCoordinate(routingManager.next_stop)
}
