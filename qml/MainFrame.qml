import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning

import GeoPositionRessource

import PostlerStorage
import ZabTable
import StopTable

MainFrameForm {

    property var selectedRoute : []


    Component.onCompleted: {
        PostlerStorage.init()

        zabTable = PostlerStorage.zabTable
        stopTable = PostlerStorage.stopTable
    }
    property ZabTable zabTable
    property StopTable stopTable


    function stopToCoordinate(stop){
        return QtPositioning.coordinate(stop.lat, stop.lon)
    }

    function next() {
        routingManager.next();
        updateRouteModel()
    }

    function before() {
        routingManager.back()
        updateRouteModel()
    }

    function nextZab() {
        routingManager.nextZab();
        updateRouteModel()
    }

    function beforeZab() {
        routingManager.backZab()
        updateRouteModel()
    }


    function updateRouteModel() {
        routeQuery.clearWaypoints();
        routeQuery.addWaypoint(GeoPositionRessource.coordinate)
        routeQuery.addWaypoint(stopToCoordinate(routingManager.cur_stop))
        routeQuery.addWaypoint(stopToCoordinate(routingManager.next_stop));
        routeModel.update()
    }



    function calculateRoute() {

        routeQuery.clearWaypoints();
        mapCenter = stopToCoordinate(routingManager.cur_stop)
        routeQuery.addWaypoint(stopToCoordinate(routingManager.cur_stop))
        routeQuery.addWaypoint(stopToCoordinate(routingManager.next_stop));


        /*

        stackView.clear()
        stackView.push(postlerMapComponent)
        mapCenter = QtPositioning.coordinate(
                        routingManager.lat,
                        routingManager.lon,
                    )
        for (var i = 0; i < route.length; i++) {
            routeQuery.addWaypoint(route[i]);
        }
        routeModel.update()
        console.log(route[0].lat)
        console.log(route[0].lon)
        */
    }



    property var waypoints : []

    RouteModel {
        id: routeModel
        property alias routeQuery : routeQuery
        query: RouteQuery{
            id: routeQuery
        }
        plugin: osmPlugin
    }

    Plugin {
        id: osmPlugin
        name: "osm"

        PluginParameter {
            name: "osm.mapping.custom.host"

            // OSM plugin will auto-append if .png isn't suffix, and that screws up apikey which silently
            // fails authentication (only Wireshark revealed it)
            value: "http://tile.thunderforest.com/atlas/%z/%x/%y.png?apikey=5e174dbc86e5477b90da4369fabe46f5&fake=.png"

            //value: "http:192.168.178.23:7070/tile/%z/%x/%y.png"
        }
    }

    property int lastIndex : 0

    property string selectedPostCode;

    function changeStackComponent(stackViewComponent) {
        stackView.clear();
        stackView.push(stackViewComponent);
    }

    pageTabBar.onCurrentIndexChanged: {
        switch(pageTabBar.currentIndex) {
        case 0:
            changeStackComponent(postlerMapComponent);
            break;
        case 1:
            changeStackComponent(postlerPlanComponent);
            break;
        case 2:
            changeStackComponent(postlerStorageComponent);
            break;
        }
    }
}
