import QtQuick 2.15

import ZabTable
import StopTable

import PostlerStorage


ZabForm {

    property var stopModel

    function removeStop(index) {
        stopModel.removeZabStop(index)
    }

    function addStop(index) {
        stopModel.addZabStop(index)
    }

    property int defaultHeight: headerGrid.implicitHeight * 1.5
    property string backgroundColor: "#4b4a4a"

    property bool isUnfold: false




    width: parent? parent.width : 0
    height: {
        if(!isUnfold)
            return defaultHeight
        else {
            return defaultHeight + stopModel.rowCount() * 125
        }
    }


    btnDelete.onClicked: {
    }

    btnUnfold.onClicked: {
        isUnfold = !isUnfold
        if(isUnfold){
            console.log(index)
        }
    }

}
