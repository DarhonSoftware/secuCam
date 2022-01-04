import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Loader {
    id: id_loader

    property string p_text
    property var p_model
    property string p_type

    signal selected(int id,string name)

    function open() {
        id_loader.active=true
        id_loader.item.p_type=id_loader.p_type
        id_loader.item.p_text=id_loader.p_text
        id_loader.item.p_model=id_loader.p_model
        id_loader.item.open()
    }

    anchors.fill: parent
    sourceComponent: id_popup
    active: false

    Component {
        id: id_popup

        Popup {
            id: id_item

            property alias p_text: id_titleText.text
            property alias p_model: id_listView.model
            property string p_type

            width: parent.width-id_labelRef.height*4
            height: parent.height-id_labelRef.height
            leftMargin: id_labelRef.height*2
            rightMargin: id_labelRef.height*2
            topMargin: id_labelRef.height
            bottomMargin: id_labelRef.height*2
            modal: true

            Rectangle {
                id: id_title
                anchors.top: parent.top
                width: parent.width
                height: id_titleText.height*2
                color: "#494949"
                Label {
                    id: id_titleText
                    anchors.centerIn: parent
                    font.bold: true
                    color: "#e5e5e5"
                }
            }

            Row {
                anchors.top: id_title.bottom
                anchors.topMargin: id_labelRef.height*0.5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                spacing: id_labelRef.height

                ListView {
                    id: id_listView
                    width: parent.width
                    height: parent.height
                    delegate: id_delegate
                    clip: true
                }
            }

            Label {id: id_labelRef; visible: false; text: "WWW"}

            Component {
                id: id_delegate

                Item {
                    id: id_margin
                    width: parent.width
                    height: id_labelDel.height*1.8

                    Label {
                        id: id_labelDel
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: id_labelRef.height
                        color: "black"
                        text: p_type=="model1" ? (role_name==undefined ? "" : role_name) : (p_type=="model2" ? role_c1 : modelData)
                        z: 1
                    }

                    Rectangle {
                        id: id_highlight
                        anchors.fill: parent
                        color: id_appWindow.p_accent
                        opacity: 0
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (p_type=="model1") id_loader.selected(role_idx,role_name)
                            if (p_type=="model2") id_loader.selected(role_c0,role_c1)
                            if (p_type=="model3") id_loader.selected(index,modelData)
                            id_item.close()
                        }
                        onPressed: {id_highlight.opacity=1; id_labelDel.color="white"}
                        onReleased: {id_highlight.opacity=0; id_labelDel.color="black"}
                        onCanceled: {id_highlight.opacity=0; id_labelDel.color="black"}
                    }
                }
            }

            Component.onCompleted: {
                id_item.focus=true
            }

            Component.onDestruction: {
                close()
            }

            onClosed: {
                id_loader.active=false
            }
        }
    }
}
