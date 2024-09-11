import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

import NominatimRessource
import QtPositioning

import PostlerStorage
import ZabTable


import "../../custom"

Item {
    property var textAreaSplit : []
    property var searchList : []
    property string checkString : ""
    property var route

    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    function splitInZabs() {
        textAreaSplit = textArea.text.split(splitTextField.text)
        textArea.clear()
        textArea.text = "Zab \t Plz \t Street \t Number \t\n"
        for(var i = 1; i < textAreaSplit.length; i++) {
            textArea.text += i + ": \t"
            textArea.text += splitTextField.text + "\t"
            textArea.text += textAreaSplit[i] + ""
            textArea.text += "\n"
        }
    }

    FileDialog {
        nameFilters: ["Text files (*.txt)"]
        visible: false
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: {
            var stops = csvStops.loadCSV(selectedFile)
            nominatimRessource.search = stops;
        }
    }

    Dialog {

        id: dialog
        title: "Warning"
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        modal: true
        Text {
            id: name
            text: checkString
            color: "white"
        }

        standardButtons: Dialog.Ok | Dialog.Cancel
    }

    ScrollView {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: rectangleBottom.top
        background: Rectangle { color: "black" }
        TextArea{
            id: textArea
            wrapMode: Text.WrapAnywhere
        }
    }
    Rectangle {
        id: rectangleBottom
        x: 0
        height: 84
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        color: Material.backgroundColor
        RowLayout {
            Button {
                id:btnSplit
                text: "Split at"
                onClicked: {
                    splitInZabs()
                }
            }
            TextField {
                id: splitTextField
                text: "32584"
            }

            Button {
                id: btnCreateStops
                text: "Convert"
                onClicked: {
                    var result = PostlerStorage.convertToModels(textArea.text, splitTextField.text, NominatimRessource)
                    console.log(result.Error)
                    if(result.Error) {
                        checkString = result.Error
                        dialog.open()
                    } else {
                        var f = {}
                        console.log(TableFilter.VALUE)
                        f["val"] = result.route_id
                        f["table_role"] = ZabTable.Route
                        f["filter_t"] = PostlerStorage.Equation
                        zabTable.clearFilter()
                        zabTable.addFilter(f);
                        zabTable.select()
                        switchPage(postlerPlanEditComponent)

                    }
                }
            }
        }
    }

}
