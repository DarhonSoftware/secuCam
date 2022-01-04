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
        }
    }

    Column {
        anchors.top: parent.top
        width: parent.width
        spacing: 0

        Rectangle {
            id: id_feedback
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

        ListView {
            id: id_history
            width: parent.width
            height: id_item.height- (id_feedback.visible ? id_feedback.height : 0)
            enabled: !p_running
            visible: !p_commIssue
            model: modelHistory
            header: id_headerDelegate
            delegate: id_delegate
        }

        Component {
            id: id_headerDelegate

            Row {
                width: parent.width

                Rectangle {
                    height: id_labelRef.height*1.5
                    width: parent.width*3/8
                    border.color: "grey"
                    color: id_appWindow.p_accent
                    Label {
                        anchors.centerIn: parent
                        text: "Date/Time"
                        color: "white"
                    }
                }
                Rectangle {
                    height: id_labelRef.height*1.5
                    width: parent.width*2/8
                    border.color: "grey"
                    color: id_appWindow.p_accent
                    Label {
                        anchors.centerIn: parent
                        text: "User"
                        color: "white"
                    }
                }
                Rectangle {
                    height: id_labelRef.height*1.5
                    width: parent.width*3/8
                    border.color: "grey"
                    color: id_appWindow.p_accent
                    Label {
                        anchors.centerIn: parent
                        text: "Action"
                        color: "white"
                    }
                }
            }
        }


        Component {
            id: id_delegate

            Row {
                width: parent.width

                Rectangle {
                    height: id_labelRef.height*1.5
                    width: parent.width*3/8
                    border.color: "grey"
                    color: "white"
                    Row {
                        anchors.centerIn: parent
                        Label {
                            id: id_labelDate
                            text: globalObject.formatDate(role_c0)
                            font.pointSize: id_labelRef.font.pointSize*0.8
                        }
                        Label {
                            id: id_labelTime
                            text: " | "+globalObject.formatTime(role_c0)
                            font.pointSize: id_labelRef.font.pointSize*0.8
                        }
                    }
                }
                Rectangle {
                    height: id_labelRef.height*1.5
                    width: parent.width*2/8
                    border.color: "grey"
                    color: "white"
                    Label {
                        anchors.centerIn: parent
                        text: role_c1
                        font.pointSize: id_labelRef.font.pointSize*0.8
                    }
                }
                Rectangle {
                    height: id_labelRef.height*1.5
                    width: parent.width*3/8
                    border.color: "grey"
                    color: "white"
                    Label {
                        anchors.centerIn: parent
                        text: role_c2
                        font.pointSize: id_labelRef.font.pointSize*0.8
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
            if (globalObject.readHistory()) {
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

    Keys.onBackPressed: {
        if (id_backButton.enabled) id_backButton.clicked()
    }

    Component.onCompleted: {
        p_running=true
        id_TimerRead.running=true
        id_item.focus=true
    }
}
