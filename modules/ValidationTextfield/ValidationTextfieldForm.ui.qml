import QtQuick
import QtQuick.Controls
import QtTest



TextField {
    id: validationTextFieldForm

    placeholderText: placeholderDefault
    property alias m_toolTip : toolTip
    property alias m_errorMsgToolTip : errorMsgTooltip


    background: Rectangle {
        color: m_color
        border.width: 1
        radius: 4
        opacity: m_opacity
        border.color: "darkGrey"
    }

    ToolTip{
        id: toolTip
        text: errorMsg
        visible: false

        Timer {
            id: errorMsgTooltip
            interval: 4000 // 4 sec
            onTriggered: toolTip.visible = false
        }
    }
}
