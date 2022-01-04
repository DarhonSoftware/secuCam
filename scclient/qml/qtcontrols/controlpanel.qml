import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Page {
    id: id_item

    property bool p_running: false
    property bool p_commIssue: false

    header: ToolBar {
        id: id_toolbar
        RowLayout {
            id: id_layout
            ToolButtonDR {
                id: id_backButton
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                enabled: !p_running
                Image {
                    height: id_backButton.height*0.5
                    width: id_backButton.height*0.5
                    anchors.centerIn: parent
                    source: "qrc:/image/icons/button-back.png"
                    smooth: true
                    opacity: id_backButton.enabled ? 1 : 0.2
                }
                onClicked: id_stack.pop()
            }
            ToolButtonDR {
                id: id_buttonMode
                visible: !p_commIssue
                enabled: !p_running && !globalObject.motion
                text: globalObject.active ? qsTr("Active") : qsTr("Passive")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                onClicked: globalObject.active=!globalObject.active
            }
        }
    }

    Rectangle {
        id: id_feedback
        anchors.top: parent.top
        width: parent.width
        height: id_feedbackText.height*2
        color: "black"
        border.color: "black"
        visible: p_running
        Label {
            id: id_feedbackText
            anchors.centerIn: parent
            color: "white"
            text: qsTr("Communicating with the security camera..")
        }
    }

    Flickable {
        id: id_flkItemScroll
        anchors.top: id_feedback.bottom
        anchors.topMargin: id_labelRef.height/2
        width: parent.width
        height: parent.height-id_feedback.height-id_labelRef.height/2
        contentHeight: id_flkItem.childrenRect.height
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Column {
            id: id_flkItem
            width: id_flkItemScroll.width
            spacing: id_labelRef.height
            visible: !p_commIssue

            Button {
                id: id_motionButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width-id_labelRef.width*4
                height: id_labelRef.height*4
                enabled: !p_running
                text: globalObject.motion ? qsTr("Camera ON") : qsTr("Camera OFF")

                background: Rectangle {
                    border.color: "DimGray"
                    border.width: 2
                    radius: id_labelRef.height*1.5
                    color: globalObject.motion ? id_appWindow.p_accent : "#9E9E9E"
                    opacity: enabled ? 1.0 : 0.5
                }

                contentItem: Text {
                    color: "white"
                    font.bold: true
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: id_motionButton.text
                    opacity: enabled ? 1.0 : 0.5
                }

                onClicked: {
                    p_running=true
                    id_TimerWriteMotion.running=true
                }
            }

            Rectangle {
                id: id_frameMode
                anchors.horizontalCenter: parent.horizontalCenter
                width: id_motionButton.width
                height: id_labelMode.height*2
                color: "transparent"
                border.color: "DimGray"
                radius: id_labelRef.height

                Label {
                    id: id_labelMode
                    anchors.centerIn: parent
                    text: globalObject.active ? qsTr("Motion detection mode") : qsTr("Video stream only mode")
                    color: globalObject.motion ? id_appWindow.p_accent : "#9E9E9E"
                    font.bold: true
                }
            }

            Row {
                width: parent.width
                spacing: id_labelRef.width

                Label {
                    text: qsTr("Tilt")
                    width: parent.width/2
                    font.bold: true
                    horizontalAlignment: Text.AlignRight
                }

                Label {
                    text: Math.round(id_sliderTilt.value)
                    width: parent.width/2
                    font.bold: true
                    horizontalAlignment: Text.AlignLeft
                }
            }

            Slider {
                id: id_sliderTilt
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width-id_labelRef.width*4
                enabled: !p_running
                from: 90
                to: -90
                value: globalObject.tilt
                handle: Rectangle {
                    x: id_sliderTilt.leftPadding + id_sliderTilt.visualPosition * (id_sliderTilt.availableWidth - width)
                    y: id_sliderTilt.topPadding + id_sliderTilt.availableHeight / 2 - height / 2
                    color: id_sliderTilt.pressed ? "DimGray" : "white"
                    border.color: "black"
                    border.width: 2
                    implicitWidth: id_labelRef.height*2
                    implicitHeight: id_labelRef.height*2
                    radius: id_labelRef.height/2
                    opacity: enabled ? 1.0 : 0.5
                }
                background: Rectangle {
                    x: id_sliderTilt.leftPadding
                    y: id_sliderTilt.topPadding + id_sliderTilt.availableHeight / 2 - height / 2
                    width: id_sliderTilt.availableWidth
                    height: id_labelRef.height/2
                    color: globalObject.motion ? id_appWindow.p_accent : "#9E9E9E"
                    border.color: "black"
                    radius: id_labelRef.height/2
                    opacity: enabled ? 1.0 : 0.5
                }
                onValueChanged: {
                    if (pressed) {p_running=true;id_TimerWriteTilt.running=true}
                }
                ToolTip {
                    parent: id_sliderTilt.handle
                    visible: id_sliderTilt.pressed
                    text: -Math.round(id_sliderTilt.position*180)+90
                    background: Rectangle {
                        border.color: "black"
                        color: "DimGray"
                        radius: id_labelRef.height/2
                    }
                }
            }

            Row {
                width: parent.width
                spacing: id_labelRef.width

                Label {
                    text: qsTr("Pan")
                    width: parent.width/2
                    font.bold: true
                    horizontalAlignment: Text.AlignRight
                }

                Label {
                    text: Math.round(id_sliderPan.value)
                    width: parent.width/2
                    font.bold: true
                    horizontalAlignment: Text.AlignLeft
                }
            }

            Slider {
                id: id_sliderPan
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width-id_labelRef.width*4
                enabled: !p_running
                from: 90
                to: -90
                value: globalObject.pan
                handle: Rectangle {
                    x: id_sliderPan.leftPadding + id_sliderPan.visualPosition * (id_sliderPan.availableWidth - width)
                    y: id_sliderPan.topPadding + id_sliderPan.availableHeight / 2 - height / 2
                    color: id_sliderPan.pressed ? "DimGray" : "white"
                    border.color: "black"
                    border.width: 2
                    implicitWidth: id_labelRef.height*2
                    implicitHeight: id_labelRef.height*2
                    radius: id_labelRef.height/2
                    opacity: enabled ? 1.0 : 0.5
                }
                background: Rectangle {
                    x: id_sliderPan.leftPadding
                    y: id_sliderPan.topPadding + id_sliderPan.availableHeight / 2 - height / 2
                    width: id_sliderPan.availableWidth
                    height: id_labelRef.height/2
                    color: globalObject.motion ? id_appWindow.p_accent : "#9E9E9E"
                    border.color: "black"
                    radius: id_labelRef.height/2
                    opacity: enabled ? 1.0 : 0.5
                }
                onValueChanged: {
                    if (pressed) {p_running=true;id_TimerWritePan.running=true}
                }
                ToolTip {
                    parent: id_sliderPan.handle
                    visible: id_sliderPan.pressed
                    text: -Math.round(id_sliderPan.position*180)+90
                    background: Rectangle {
                        border.color: "black"
                        color: "DimGray"
                        radius: id_labelRef.height/2
                    }
                }
            }
        }
    }


    Label { id: id_labelRef; text: "W"; visible: false }

    MessageDialog {
        id: id_dialogOK
        p_title: qsTr("Information")
        p_buttonYes: qsTr("OK")
    }

    Timer {
        id: id_TimerRead
        interval: 500
        repeat: false
        running: false
        onTriggered: {
            var result;
            result=globalObject.readMotion();
            if (result) result=globalObject.readTilt();
            if (result) result=globalObject.readPan();
            if (result) {
                p_running=false
            }
            else {
                p_running=false
                p_commIssue=true
                id_dialogOK.p_text=globalObject.message()
                id_dialogOK.open()
            }
        }
    }

    Timer {
        id: id_TimerWriteMotion
        interval: 200
        repeat: false
        running: false
        onTriggered: {
            if (globalObject.writeMotion(!globalObject.motion,globalObject.active)) {
                p_running=false
                globalObject.motion=!globalObject.motion
            }
            else {
                p_running=false
                id_dialogOK.p_text=globalObject.message()
                id_dialogOK.open()
            }
        }
    }

    Timer {
        id: id_TimerWriteTilt
        interval: 200
        repeat: false
        running: false
        onTriggered: {
            if (globalObject.writeTilt(Math.round(id_sliderTilt.value))) {
                p_running=false
            }
            else {
                p_running=false
                id_dialogOK.p_text=globalObject.message()
                id_dialogOK.open()
            }
        }
    }

    Timer {
        id: id_TimerWritePan
        interval: 200
        repeat: false
        running: false
        onTriggered: {
            if (globalObject.writePan(Math.round(id_sliderPan.value))) {
                p_running=false
            }
            else {
                p_running=false
                id_dialogOK.p_text=globalObject.message()
                id_dialogOK.open()
            }
        }
    }

    Keys.onBackPressed: {
        if (id_backButton.enabled) id_backButton.clicked()
    }

    Component.onCompleted: {
        globalObject.motion=false
        globalObject.active=false
        globalObject.tilt=0
        globalObject.pan=0
        p_running=true
        id_TimerRead.running=true
        id_item.focus=true
    }
}
