import QtQuick 2.15

import ZabStopStates

ZabStopForm {

    property var model
    isPackage.onClicked: model.package = !model.package
    isLetter.onClicked: model.letter = !model.letter
    deleteNumber.onClicked: removeStop(index)
    addRow.onClicked: addStop(index)

    stateIcon.source:
        switch(model.zabStopState) {
        case 0:
            return "qrc:/icons/icons/error_outline.svg";
        case 3:
            return "qrc:/icons/icons/explore.svg";
        case 1:
            return "qrc:/icons/icons/check_one.svg"
        case 2:
            return "qrc:/icons/icons/check_many.svg"
        case 4:
            return "qrc:/icons/icons/error.svg"
    }
}
