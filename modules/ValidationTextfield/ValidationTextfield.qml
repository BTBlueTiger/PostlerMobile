import QtQuick 2.15
import ValidationTextfieldModel

ValidationTextfieldForm {


    ValidationTextfieldModel{ id: model }

    // Unsere propertys aus dem Model
    property alias state : model.state
    property alias m_opacity : model.opacity
    property alias m_color : model.color

    // Default its not Valid
    property bool isValid: state === 2
    property string errorMsg : ""
    property string placeholderDefault: ""

    onStateChanged: {
        if(state === 1) {
            m_toolTip.visible = true
            m_errorMsgToolTip.start()
        }
    }
}
