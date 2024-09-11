import QtQuick 2.15
import QtQuick.Controls

import ZabTable


PostlerEditPageForm {

    width: parent ? parent.width : null
    height: parent ? parent.height : null

    zabListView.model: zabTable

    zabListView.delegate: Zab {
        zabNumber: index
        streetName: street

    }
}
