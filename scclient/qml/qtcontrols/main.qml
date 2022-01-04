import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    id: id_appWindow

    property color p_accent: Material.accent
    property color p_background: Material.background

    StackView {
        id: id_stack
        anchors.fill: parent
        initialItem: "qrc:/src/qml/qtcontrols/login.qml"

        onCurrentItemChanged: id_timer.restart()
    }

    Timer {
        id: id_timer
        interval: 10*60*1000
        repeat: true
        running: true

        onTriggered: id_stack.pop(null)
    }

    Component.onCompleted: {
        width=Screen.width/4
        height=Screen.height/1.5
        x=(Screen.width-width)/2
        y=(Screen.height-height)/2
        Material.accent = Material.Indigo
        Material.primary = Material.DeepOrange
        visible=true
    }
}
