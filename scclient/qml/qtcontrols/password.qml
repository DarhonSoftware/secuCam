import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Page {
    id: id_item
    property string p_pass
    property string p_passFirst

    state: "1"

    header: ToolBar {
        id: id_toolbar
        RowLayout {
            id: id_layout
            ToolButtonDR {
                id: id_backButton
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                Image {
                    height: id_backButton.height*0.5
                    width: id_backButton.height*0.5
                    anchors.centerIn: parent
                    source: "qrc:/image/icons/button-back.png"
                    smooth: true
                }
                onClicked: id_stack.pop()
            }
            ToolButtonDR {
                id: id_buttonLogin
                text: qsTr("Login")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                onClicked: {
                    if (globalObject.isPassword(p_pass)) {
                        p_pass=""
                        id_item.state="2"
                    }
                    else {
                        p_pass=""
                        id_text.opacity=1
                    }
                }
            }
            ToolButtonDR {
                id: id_buttonSave1
                text: qsTr("Save")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                onClicked: {
                    if (p_pass!="") {
                        p_passFirst=p_pass
                        p_pass=""
                        id_item.state="3"
                    }
                    else {
                        id_text.opacity=1
                    }
                }
            }
            ToolButtonDR {
                id: id_buttonSave2
                text: qsTr("Save")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                onClicked: {
                    if (p_pass==p_passFirst) {
                        globalObject.setPassword(p_pass)
                        id_stack.pop()
                    }
                    else {
                        p_pass=""
                        id_text.opacity=1
                    }
                }
            }
        }
    }

    Label { id: id_labelRef; text: "W"; visible: false }

    Label {
        id: id_text
        anchors.top: parent.top
        anchors.topMargin: height
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Grid {
        id: id_grid
        anchors.top: id_text.bottom
        anchors.topMargin: id_text.height
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 3
        rows: 4
        spacing: id_labelRef.height*0.5

        Repeater {
            model: 9
            Button {
                text: index+1
                width: (id_item.width/3 - 2*id_grid.spacing)
                onClicked: {p_pass+=text; id_text.opacity=0;}
            }

        }

        Button {
            text: "*"
            width: (id_item.width/3 - 2*id_grid.spacing)
            onClicked: {p_pass+=text; id_text.opacity=0;}
        }

        Button {
            text: "0"
            width: (id_item.width/3 - 2*id_grid.spacing)
            onClicked: {p_pass+=text; id_text.opacity=0;}
        }

        Button {
            text: "#"
            width: (id_item.width/3 - 2*id_grid.spacing)
            onClicked: {p_pass+=text; id_text.opacity=0;}
        }
    }

    states: [
        State {
            name: "1"
            PropertyChanges { target: id_text; text: qsTr("Type current password and click 'Login'")}
            PropertyChanges { target: id_text; opacity: 1}
            PropertyChanges { target: id_buttonLogin; visible: true}
            PropertyChanges { target: id_buttonSave1; visible: false}
            PropertyChanges { target: id_buttonSave2; visible: false}
        },
        State {
            name: "2"
            PropertyChanges { target: id_text; text: qsTr("Type a new password and click 'Save'")}
            PropertyChanges { target: id_text; opacity: 1}
            PropertyChanges { target: id_buttonLogin; visible: false}
            PropertyChanges { target: id_buttonSave1; visible: true}
            PropertyChanges { target: id_buttonSave2; visible: false}
        },
        State {
            name: "3"
            PropertyChanges { target: id_text; text: qsTr("Type again to confirm and click 'Save'")}
            PropertyChanges { target: id_text; opacity: 1}
            PropertyChanges { target: id_buttonLogin; visible: false}
            PropertyChanges { target: id_buttonSave1; visible: false}
            PropertyChanges { target: id_buttonSave2; visible: true}
        }
    ]

    Keys.onBackPressed: {
        id_backButton.clicked()
    }

    Component.onCompleted: {
        id_item.focus=true
    }
}
