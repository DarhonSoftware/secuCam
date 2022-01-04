import QtQuick 2.7
import QtQuick.Controls 2.3

ToolButton {
    id: id_control
    width: 120

    contentItem: Text {
        text: id_control.text
        font: id_control.font
        opacity: enabled ? 1.0 : 0.3
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 48
        implicitHeight: 48
        color: Qt.darker("#33333333", id_control.enabled && (id_control.checked || id_control.highlighted) ? 1.5 : 1.0)
        opacity: enabled ? 1 : 0.3
        visible: id_control.down || (id_control.enabled && (id_control.checked || id_control.highlighted))
    }
}
