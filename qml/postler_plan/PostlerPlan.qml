import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts


import "../dialog"


PostlerPlanForm {

    Dialog {

        id: dialog
        title: "Title"
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        modal: true
        TextField{
            placeholderText: "Name"
        }

        standardButtons: Dialog.Ok | Dialog.Cancel
    }

    function switchPage(page) {
        postlerPlanStackView.clear()
        postlerPlanStackView.push(page)

        if(page === postlerPlanLoadComponent) {
            postlerPlanTabBar.setCurrentIndex(0);
        } else if(page === postlerPlanEditComponent) {
            postlerPlanTabBar.setCurrentIndex(1);
        }
    }

    tabEdit.onClicked: {
        postlerPlanStackView.clear()
        postlerPlanStackView.push(postlerPlanEditComponent)
    }

    tabLoad.onClicked: {
        postlerPlanStackView.clear()
        postlerPlanStackView.push(postlerPlanLoadComponent)
    }

    tabSave.onClicked: {
        dialog.open()
        postlerPlanTabBar.setCurrentIndex(1);
    }
}
