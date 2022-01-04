import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.0

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
                id: id_applyButton
                text: qsTr("Apply")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                enabled: !p_running
                visible: !p_commIssue
                onClicked: {
                    //Validate data to be saved
                    if ((id_fieldCommPass.text==="")||(id_fieldUsername.text==="")) {
                        id_dialogOK.p_text=qsTr("The camera password and username must be filled.")
                        id_dialogOK.open()
                        return
                    }

                    //Write data to server
                    p_running=true
                    id_TimerWrite.running=true
                }
            }
        }
    }

    ColumnLayout {
        anchors.top: parent.top
        width: id_item.width
        spacing: 0

        Rectangle {
            id: id_feedback
            height: id_feedbackText.height*2
            color: "black"
            border.color: "black"
            visible: p_running
            Layout.fillWidth: true
            Label {
                id: id_feedbackText
                anchors.centerIn: parent
                color: "white"
                text: qsTr("Communicating with the security camera..")
            }
        }

        TabBar {
            id: id_tabBar
            enabled: !p_running
            visible: !p_commIssue
            Layout.fillWidth: true

            TabButton {
                text: qsTr("Communication")
            }
            TabButton {
                text: qsTr("Notification")
            }
        }

        StackLayout {
            id: id_stackLayout
            enabled: !p_running
            visible: !p_commIssue
            currentIndex: id_tabBar.currentIndex
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.leftMargin: id_labelRef.height
            Layout.rightMargin: id_labelRef.height

            Flickable {
                id: id_scrollComm
                anchors.fill: id_stackLayout

                Item {
                    width: id_scrollComm.width
                    height: childrenRect.height

                    //FIELD 1
                    RowLayout {
                        id: id_rowUsername
                        spacing: id_labelRef.height
                        height: id_fieldUsername.height+id_labelRef.height

                        Label {
                            id: id_labelUsername
                            text: " "+qsTr("Username")
                            Layout.alignment: Qt.AlignVCenter
                        }

                        TextField {
                            id: id_fieldUsername
                            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                            text: globalObject.username
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                        }
                    }

                    //FIELD 2
                    RowLayout {
                        id: id_rowCommPass
                        spacing: id_labelRef.height
                        height: id_fieldCommPass.height+id_labelRef.height

                        Label {
                            id: id_labelCommPass
                            text: " "+qsTr("Password")
                            Layout.alignment: Qt.AlignVCenter
                        }

                        TextField {
                            id: id_fieldCommPass
                            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                            text: globalObject.commPass
                            echoMode: TextInput.Password
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                        }
                    }

                    //FIELD 3
                    RowLayout {
                        id: id_rowUrlHostIP
                        spacing: id_labelRef.height
                        height: id_fieldUrlHostIP.height+id_labelRef.height

                        Label {
                            id: id_labelUrlHostIP
                            Layout.alignment: Qt.AlignVCenter
                            text: " "+qsTr("URL")
                        }

                        TextField {
                            id: id_fieldUrlHostIP
                            inputMethodHints: Qt.ImhUrlCharactersOnly
                            text: globalObject.urlHostIP
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                        }
                    }

                    //FIELD 4
                    RowLayout {
                        id: id_rowUrlDDNS
                        spacing: id_labelRef.height
                        height: id_fieldUrlDDNS.height+id_labelRef.height

                        Label {
                            id: id_labelUrlDDNS
                            Layout.alignment: Qt.AlignVCenter
                            text: " "+qsTr("DDNS")
                        }

                        TextField {
                            id: id_fieldUrlDDNS
                            inputMethodHints: Qt.ImhUrlCharactersOnly
                            text: globalObject.urlDDNS
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                        }
                    }

                    states: [
                        State {
                            name: "PORTRAIT"
                            when: id_item.Screen.primaryOrientation===Qt.PortraitOrientation
                            AnchorChanges {
                                target: id_rowUsername
                                anchors.left: parent.left
                                anchors.top: parent.top
                            }
                            AnchorChanges {
                                target: id_rowCommPass
                                anchors.left: parent.left
                                anchors.top: id_rowUsername.bottom
                            }
                            AnchorChanges {
                                target: id_rowUrlHostIP
                                anchors.left: parent.left
                                anchors.top: id_rowCommPass.bottom
                            }
                            AnchorChanges {
                                target: id_rowUrlDDNS
                                anchors.left: parent.left
                                anchors.top: id_rowUrlHostIP.bottom
                            }
                            PropertyChanges {
                                target: id_rowUsername
                                width: parent.width
                            }
                            PropertyChanges {
                                target: id_rowCommPass
                                width: parent.width
                            }
                            PropertyChanges {
                                target: id_rowUrlHostIP
                                width: parent.width
                            }
                            PropertyChanges {
                                target: id_rowUrlDDNS
                                width: parent.width
                            }
                        },

                        State {
                            name: "LANDSCAPE"
                            when: id_item.Screen.primaryOrientation===Qt.LandscapeOrientation
                            AnchorChanges {
                                target: id_rowUsername
                                anchors.left: parent.left
                                anchors.top: parent.top
                            }
                            AnchorChanges {
                                target: id_rowCommPass
                                anchors.left: id_rowUsername.right
                                anchors.verticalCenter: id_rowUsername.verticalCenter
                            }
                            AnchorChanges {
                                target: id_rowUrlHostIP
                                anchors.left: parent.left
                                anchors.top: id_rowUsername.bottom
                            }
                            AnchorChanges {
                                target: id_rowUrlDDNS
                                anchors.left: id_rowUrlHostIP.right
                                anchors.verticalCenter: id_rowUrlHostIP.verticalCenter
                            }
                            PropertyChanges {
                                target: id_rowUsername
                                width: parent.width/2
                            }
                            PropertyChanges {
                                target: id_rowCommPass
                                width: parent.width/2
                            }
                            PropertyChanges {
                                target: id_rowUrlHostIP
                                width: parent.width/2
                            }
                            PropertyChanges {
                                target: id_rowUrlDDNS
                                width: parent.width/2
                            }
                        }
                    ]
                }
            }

            Flickable {
                id: id_scrollNotif
                anchors.fill: id_stackLayout

                Item {
                    width: id_scrollNotif.width
                    height: childrenRect.height

                    //FIELD 1
                    RowLayout {
                        id: id_rowSmtpUser
                        spacing: id_labelRef.height
                        height: id_fieldSmtpUser.height+id_labelRef.height

                        Label {
                            id: id_labelSmtpUser
                            text: " "+qsTr("Email")
                            Layout.alignment: Qt.AlignVCenter
                        }

                        TextField {
                            id: id_fieldSmtpUser
                            inputMethodHints: Qt.ImhEmailCharactersOnly
                            text: globalObject.smtpUser
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                        }
                    }

                    //FIELD 2
                    RowLayout {
                        id: id_rowSmtpPass
                        spacing: id_labelRef.height
                        height: id_fieldSmtpPass.height+id_labelRef.height

                        Label {
                            id: id_labelSmtpPass
                            text: " "+qsTr("Password")
                            Layout.alignment: Qt.AlignVCenter
                        }

                        TextField {
                            id: id_fieldSmtpPass
                            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                            text: globalObject.smtpPass
                            echoMode: TextInput.Password
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                        }
                    }

                    //FIELD 3
                    RowLayout {
                        id: id_rowSmtpHost
                        spacing: id_labelRef.height
                        height: id_fieldSmtpHost.height+id_labelRef.height

                        Label {
                            id: id_labelSmtpHost
                            text: " "+qsTr("Server")
                            Layout.alignment: Qt.AlignVCenter
                        }

                        TextField {
                            id: id_fieldSmtpHost
                            inputMethodHints: Qt.ImhUrlCharactersOnly
                            text: globalObject.smtpHost
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                        }
                    }

                    //FIELD 4
                    RowLayout {
                        id: id_rowSmtpPort
                        spacing: id_labelRef.height
                        height: id_fieldSmtpPort.height+id_labelRef.height

                        Label {
                            id: id_labelSmtpPort
                            text: " "+qsTr("Port")
                            Layout.alignment: Qt.AlignVCenter
                        }

                        TextField {
                            id: id_fieldSmtpPort
                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: IntValidator { bottom: 0 }
                            text: globalObject.smtpPort
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                        }
                    }

                    //FIELD 5
                    RowLayout {
                        id: id_rowSmtpConnType
                        spacing: id_labelRef.height
                        height: id_fieldSmtpConnType.height+id_labelRef.height

                        Label {
                            id: id_labelSmtpConnType
                            text: " "+qsTr("Connection")
                            Layout.alignment: Qt.AlignVCenter
                        }

                        Button {
                            id: id_fieldSmtpConnType
                            text: globalObject.smtpConnType
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                            onClicked: id_dialogConnType.open()
                        }
                    }

                    //FIELD 6
                    RowLayout {
                        id: id_rowSmtpAuthMethod
                        spacing: id_labelRef.height
                        height: id_fieldSmtpAuthMethod.height+id_labelRef.height

                        Label {
                            id: id_labelSmtpAuthMethod
                            text: " "+qsTr("Authorization")
                            Layout.alignment: Qt.AlignVCenter
                        }

                        Button {
                            id: id_fieldSmtpAuthMethod
                            text: globalObject.smtpAuthMethod
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                            onClicked: id_dialogAuthMethod.open()
                        }
                    }

                    states: [
                        State {
                            name: "PORTRAIT"
                            when: id_item.Screen.primaryOrientation==Qt.PortraitOrientation
                            AnchorChanges {
                                target: id_rowSmtpUser
                                anchors.left: parent.left
                                anchors.top: parent.top
                            }
                            AnchorChanges {
                                target: id_rowSmtpPass
                                anchors.left: parent.left
                                anchors.top: id_rowSmtpUser.bottom
                            }
                            AnchorChanges {
                                target: id_rowSmtpHost
                                anchors.left: parent.left
                                anchors.top: id_rowSmtpPass.bottom
                            }
                            AnchorChanges {
                                target: id_rowSmtpPort
                                anchors.left: parent.left
                                anchors.top: id_rowSmtpHost.bottom
                            }
                            AnchorChanges {
                                target: id_rowSmtpConnType
                                anchors.left: parent.left
                                anchors.top: id_rowSmtpPort.bottom
                            }
                            AnchorChanges {
                                target: id_rowSmtpAuthMethod
                                anchors.left: parent.left
                                anchors.top: id_rowSmtpConnType.bottom
                            }
                            PropertyChanges {
                                target: id_rowSmtpUser
                                width: parent.width
                            }
                            PropertyChanges {
                                target: id_rowSmtpPass
                                width: parent.width
                            }
                            PropertyChanges {
                                target: id_rowSmtpHost
                                width: parent.width
                            }
                            PropertyChanges {
                                target: id_rowSmtpPort
                                width: parent.width
                            }
                            PropertyChanges {
                                target: id_rowSmtpConnType
                                width: parent.width
                            }
                            PropertyChanges {
                                target: id_rowSmtpAuthMethod
                                width: parent.width
                            }
                        },

                        State {
                            name: "LANDSCAPE"
                            when: id_item.Screen.primaryOrientation==Qt.LandscapeOrientation
                            AnchorChanges {
                                target: id_rowSmtpUser
                                anchors.left: parent.left
                                anchors.top: parent.top
                            }
                            AnchorChanges {
                                target: id_rowSmtpPass
                                anchors.left: id_rowSmtpUser.right
                                anchors.verticalCenter: id_rowSmtpUser.verticalCenter
                            }
                            AnchorChanges {
                                target: id_rowSmtpHost
                                anchors.left: parent.left
                                anchors.top: id_rowSmtpUser.bottom
                            }
                            AnchorChanges {
                                target: id_rowSmtpPort
                                anchors.left: id_rowSmtpHost.right
                                anchors.verticalCenter: id_rowSmtpHost.verticalCenter
                            }
                            AnchorChanges {
                                target: id_rowSmtpConnType
                                anchors.left: parent.left
                                anchors.top: id_rowSmtpHost.bottom
                            }
                            AnchorChanges {
                                target: id_rowSmtpAuthMethod
                                anchors.left: id_rowSmtpConnType.right
                                anchors.verticalCenter: id_rowSmtpConnType.verticalCenter
                            }
                            PropertyChanges {
                                target: id_rowSmtpUser
                                width: parent.width/2
                            }
                            PropertyChanges {
                                target: id_rowSmtpPass
                                width: parent.width/2
                            }
                            PropertyChanges {
                                target: id_rowSmtpHost
                                width: parent.width/2
                            }
                            PropertyChanges {
                                target: id_rowSmtpPort
                                width: parent.width/2
                            }
                            PropertyChanges {
                                target: id_rowSmtpConnType
                                width: parent.width/2
                            }
                            PropertyChanges {
                                target: id_rowSmtpAuthMethod
                                width: parent.width/2
                            }
                        }
                    ]
                }
            }
        }

        Label { id: id_labelRef; text: "W"; visible: false }

        MessageDialog {
            id: id_dialogOK
            p_title: qsTr("Information")
            p_buttonYes: qsTr("OK")
        }

        ListDialog {
            id: id_dialogConnType
            p_type: "model3"
            p_text: qsTr("Connection")
            p_model: modelConnType
            onSelected: {
                globalObject.smtpConnType=name
                globalObject.setSmtpConnType(id)
            }
        }

        ListDialog {
            id: id_dialogAuthMethod
            p_type: "model3"
            p_text: qsTr("Authorization")
            p_model: modelAuthMethod
            onSelected: {
                globalObject.smtpAuthMethod=name
                globalObject.setSmtpAuthMethod(id)
            }
        }

        Timer {
            id: id_TimerRead
            interval: 500
            repeat: false
            running: false
            onTriggered: {
                if (globalObject.readSetup()) {
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
            id: id_TimerWrite
            interval: 200
            repeat: false
            running: false
            onTriggered: {
                if (globalObject.writeSetup(id_fieldUrlDDNS.text,id_fieldSmtpHost.text,id_fieldSmtpUser.text,id_fieldSmtpPass.text,
                                            id_fieldSmtpPort.text,id_fieldCommPass.text,id_fieldUsername.text,id_fieldUrlHostIP.text)) {
                    p_running=false
                    id_stack.pop()
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
            p_running=true
            id_TimerRead.running=true
            id_item.focus=true
        }
    }
}
