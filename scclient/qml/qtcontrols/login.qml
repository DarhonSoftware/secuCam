import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Page {
    id: id_item
    property string p_pass

    header: ToolBar {
        id: id_toolbar
        RowLayout {
            id: id_layout
            ToolButtonDR {
                id: id_buttonQuit
                text: qsTr("Quit")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                onClicked: Qt.quit()
            }
            ToolButtonDR {
                id: id_buttonLogin
                text: qsTr("Login")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                onClicked: {
                    if (p_pass!="") {
                        if (globalObject.isPassword(p_pass)) {
                            id_stack.push("qrc:/src/qml/qtcontrols/initpage.qml");
                        }
                        else {
                            id_dialogOK.p_text=globalObject.message();
                            id_dialogOK.open();
                        }
                        p_pass="";
                    }
                }
            }
            ToolButtonDR {
                id: id_buttonAbout
                text: qsTr("About")
                Layout.preferredWidth: (id_item.width-id_layout.spacing*3)/4
                onClicked: id_stack.push("qrc:/src/qml/qtcontrols/about.qml")
            }
        }
    }

    Label { id: id_labelRef; text: "W"; visible: false }

    Grid {
        id: id_grid
        anchors.centerIn: parent
        columns: 3
        rows: 4
        spacing: id_labelRef.height*0.4

        Repeater {
            model: 9
            Button {
                text: index+1
                width: (id_item.width/3 - 2*id_grid.spacing)
                onClicked: p_pass+=text
            }
        }

        Button {
            text: "*"
            width: (id_item.width/3 - 2*id_grid.spacing)
            onClicked: p_pass+=text
        }

        Button {
            text: "0"
            width: (id_item.width/3 - 2*id_grid.spacing)
            onClicked: p_pass+=text
        }

        Button {
            text: "#"
            width: (id_item.width/3 - 2*id_grid.spacing)
            onClicked: p_pass+=text
        }
    }

    MessageDialog {
        id: id_dialogOK
        p_title: qsTr("Information")
        p_buttonYes: qsTr("OK")
    }

    Keys.onBackPressed: {
        id_buttonQuit.clicked()
    }

    StackView.onStatusChanged: {
        if (StackView.status==StackView.Activating) {
            if (!globalObject.existPassword()) {
                id_dialogOK.p_text=qsTr("Your application is opening for the first time.\nChoose a password you will remember.\nIf you forget your password, you won't be able to access again.");
                id_dialogOK.open();
            }
            id_item.focus=true
        }
    }
}
