import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Loader {
    id: id_loader

    property string p_title
    property string p_text
    property string p_buttonYes
    property string p_buttonNo

    signal yes()
    signal no()

    function open() {
        id_loader.active=true
        id_loader.item.p_title=id_loader.p_title
        id_loader.item.p_text=id_loader.p_text
        id_loader.item.p_buttonYes=id_loader.p_buttonYes
        id_loader.item.p_buttonNo=id_loader.p_buttonNo
        id_loader.item.open()
    }

    anchors.fill: parent
    sourceComponent: id_popup
    active: false

    Component {
        id: id_popup

        Popup {
            id: id_item

            property alias p_title: id_title.text
            property alias p_text: id_text.text
            property alias p_buttonYes: id_buttonYes.text
            property alias p_buttonNo: id_buttonNo.text

            width: parent.width*0.7
            height: id_column.height+id_labelRef.height
            leftMargin: parent.width*0.3*0.5
            rightMargin: parent.width*0.3*0.5
            modal: true
            background: Rectangle {
                color: id_appWindow.p_background
            }

            Column {
                id: id_column
                width: parent.width

                Rectangle {
                    height: id_title.height*2
                    width: parent.width
                    color: "#494949"

                    Label {
                        id: id_title
                        anchors.centerIn: parent
                        font.bold: true
                        color: "#e5e5e5"
                    }
                }

                Item {
                    height: id_labelRef.height
                    width: parent.width
                }

                TextEdit {
                    id: id_text
                    width: parent.width
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                }

                Item {
                    height: id_labelRef.height
                    width: parent.width
                }

                Rectangle {
                    height: 2
                    width: parent.width
                    border.color: "black"
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: id_column.width/6

                    Button {
                        id: id_buttonNo
                        width: id_column.width/3
                        visible: text!=""
                        onClicked: {
                            id_loader.no()
                            id_item.close()
                        }
                    }
                    Button {
                        id: id_buttonYes
                        width: id_column.width/3
                        visible: text!=""
                        onClicked: {
                            id_loader.yes()
                            id_item.close()
                        }
                    }
                }
            }

            Label {id: id_labelRef; text: "W"; visible:false;}

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
