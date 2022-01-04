import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3


Page {
    id: id_item

    property bool p_paramVisible

    header: ToolBar {
        id: id_toolbar
        RowLayout {
            id: id_layout
            ToolButtonDR {
                id: id_buttonExit
                text: qsTr("Exit")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                onClicked: id_stack.pop()
            }
            ToolButtonDR {
                id: id_buttonPassword
                text: qsTr("Pass")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                onClicked: id_stack.push("qrc:/src/qml/qtcontrols/password.qml")
            }
            ToolButtonDR {
                id: id_buttonParam
                checkable: true
                checked: p_paramVisible
                text: qsTr("Param")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                onClicked: p_paramVisible=!p_paramVisible
            }
        }
    }

    Label { id: id_labelRef; text: "W"; visible: false }

    Flickable {
        id: id_flkItemScroll
        anchors.fill: parent
        contentHeight: id_flkItem.childrenRect.height
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Item {
            id: id_flkItem
            width: parent.width
            height: childrenRect.height

            Column {
                id: id_main
                anchors.top: parent.top
                anchors.topMargin: id_labelRef.height
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width-id_labelRef.width
                spacing: 5

                Rectangle {
                    id: id_title
                    width: parent.width
                    height: id_titleText.height*2
                    color: id_appWindow.p_accent
                    visible: p_paramVisible
                    Label {
                        id: id_titleText
                        anchors.centerIn: parent
                        font.bold: true
                        text: qsTr("Communication Parameters")
                        color: "white"
                    }
                }

                Label {
                    id: id_urlHostIPLabel
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    visible: p_paramVisible
                    text: qsTr("Url for security camera")
                }

                RowLayout {
                    id: id_urlHostIPRow
                    width: parent.width
                    spacing: id_labelRef.width
                    visible: p_paramVisible

                    Button {
                        id: id_urlHostIPButton
                        Layout.preferredWidth: id_item.width/3
                        text: qsTr("Default")
                        onClicked: id_urlHostIPText.text=globalObject.urlHostIP
                    }
                    TextField {
                        id: id_urlHostIPText
                        inputMethodHints: Qt.ImhUrlCharactersOnly
                        Layout.fillWidth: true
                        text: globalObject.urlHostIPSel
                    }
                }

                Label {
                    id: id_tcpLabel
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    visible: p_paramVisible
                    text: qsTr("Communication password & port")
                }

                RowLayout {
                    id: id_tcpRow
                    width: parent.width
                    spacing: id_labelRef.width
                    visible: p_paramVisible

                    Button {
                        id: id_tcpCommPassButton
                        Layout.preferredWidth: id_item.width/3
                        text: qsTr("Default")
                        onClicked: id_tcpCommPassText.text=globalObject.commPass
                    }
                    TextField {
                        id: id_tcpCommPassText
                        inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                        Layout.fillWidth: true
                        echoMode: TextInput.Password
                        text: globalObject.commPassSel
                    }
                    TextField {
                        id: id_tcpPortText
                        inputMethodHints: Qt.ImhDigitsOnly
                        validator: IntValidator { bottom: 0 }
                        Layout.fillWidth: true
                        text: globalObject.tcpPortSel
                    }
                }

                Item {
                    width: parent.width
                    height: id_title.height
                    visible: p_paramVisible

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.width
                        height: 2
                        color: id_appWindow.p_accent
                    }
                }

                Button {
                    id: id_ControlPanelButton
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: id_item.width/2
                    text: qsTr("Control Panel")
                    onClicked: {
                        if ((id_urlHostIPText.text!="")&&(id_tcpCommPassText.text!="")&&(id_tcpPortText.text!="")) {
                            globalObject.urlHostIPSel=id_urlHostIPText.text
                            globalObject.commPassSel=id_tcpCommPassText.text
                            globalObject.tcpPortSel=id_tcpPortText.text
                            id_stack.push("qrc:/src/qml/qtcontrols/controlpanel.qml")
                        }
                        else {
                            id_dialogOK.p_text=qsTr("All parameters need to be filled.")
                            id_dialogOK.open()
                        }
                    }
                }

                Button {
                    id: id_SettingsButton
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: id_item.width/2
                    text: qsTr("Settings")
                    onClicked: {
                        if ((id_urlHostIPText.text!="")&&(id_tcpCommPassText.text!="")&&(id_tcpPortText.text!="")) {
                            globalObject.urlHostIPSel=id_urlHostIPText.text
                            globalObject.commPassSel=id_tcpCommPassText.text
                            globalObject.tcpPortSel=id_tcpPortText.text
                            id_stack.push("qrc:/src/qml/qtcontrols/settings.qml")
                        }
                        else {
                            id_dialogOK.p_text=qsTr("All parameters need to be filled.")
                            id_dialogOK.open()
                        }
                    }
                }

                Button {
                    id: id_HistoyButton
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: id_item.width/2
                    text: qsTr("History")
                    onClicked: {
                        if ((id_urlHostIPText.text!="")&&(id_tcpCommPassText.text!="")&&(id_tcpPortText.text!="")) {
                            globalObject.urlHostIPSel=id_urlHostIPText.text
                            globalObject.commPassSel=id_tcpCommPassText.text
                            globalObject.tcpPortSel=id_tcpPortText.text
                            id_stack.push("qrc:/src/qml/qtcontrols/history.qml")
                        }
                        else {
                            id_dialogOK.p_text=qsTr("All parameters need to be filled.")
                            id_dialogOK.open()
                        }
                    }
                }

            }
        }
    }

    MessageDialog {
        id: id_dialogOK
        p_title: qsTr("Information")
        p_buttonYes: qsTr("OK")
    }

    Keys.onBackPressed: {
        id_buttonExit.clicked()
    }

    Component.onCompleted: {
        globalObject.initInitpage()
        p_paramVisible=!globalObject.validateParam()
    }

    StackView.onStatusChanged: {
        if (StackView.status==StackView.Activating) {
            id_item.focus=true
        }
    }
}
