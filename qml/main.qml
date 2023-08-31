import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels 1.0

ApplicationWindow  {
    width: 1280
    height: 480
    visible: true
    title: qsTr("SerialPort Reader")

    property int table_row_height: 40
    property int record_dlg_height: 20

    TableView {
        id: tableView
        width: parent.width/2
        anchors {
            top: parent.top
            topMargin: 5
            left: parent.left
            bottom: ctrl.top
            bottomMargin: 30
        }
        columnSpacing: 1
        rowSpacing: 1
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        model: APP_MODEL.tableModel

        delegate:  TextInput {
                    text: String(model.display)
                    padding: 12
                    selectByMouse: true
                    onAccepted: model.display = text
                    Rectangle {
                        anchors.fill: parent
                        color: "#efefef"
                        z: -1
                    }
                    onWidthChanged: {

                    }
                }

        onHeightChanged: {
            APP_MODEL.makeTableModel(height,table_row_height)
        }

        ScrollBar {
            id: vbarForTable
            hoverEnabled: true
            active: true//hovered || pressed
            orientation: Qt.Vertical
            size: APP_MODEL.dataSize <= 200? 1 : (200 / APP_MODEL.dataSize)
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            onPositionChanged: {
                if (position === 1.0) APP_MODEL.pointerIndexForTable = 0
                else APP_MODEL.pointerIndexForTable =  parseInt(APP_MODEL.dataSize * position, 10)
            }

            Connections {
                target: APP_MODEL
                function onPointerIndexChanged() {
                    vbar.position = APP_MODEL.pointerIndexForTable/APP_MODEL.dataSize
                }
            }

        }
    }

    HorizontalHeaderView {
        id: verticalHeader
        syncView: tableView
        anchors.left: tableView.left
        clip: true
    }

    ListView {
        id: contentList
        width: parent.width/2
        anchors {
            top: parent.top
            topMargin: 5
            right: parent.right
            bottom: ctrl.top
            bottomMargin: 30
        }

        model: APP_MODEL.recordList
        interactive: false
        delegate: Item {
            id: dlg
            width: contentList.width
            height: record_dlg_height
            Text {
                anchors.fill: parent
                text: dlg.visible? modelData.raw : ""
                padding: 5
                elide: Text.ElideRight
            }
            Rectangle{
                width: parent.width
                height: 1
                color: "green"
            }

            onVisibleChanged: {
                modelData.visible = visible
            }

            Component.onCompleted: {
                modelData.visible = visible
            }
        }


        ScrollBar {
            id: vbar
            hoverEnabled: true
            active: true//hovered || pressed
            orientation: Qt.Vertical
            size: APP_MODEL.dataSize <= 200? 1 : (200 / APP_MODEL.dataSize)
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            onPositionChanged: {
                if (position === 1.0) APP_MODEL.pointerIndex = 0
                else APP_MODEL.pointerIndex =  parseInt(APP_MODEL.dataSize * position, 10)
            }

            Connections {
                target: APP_MODEL
                function onPointerIndexChanged() {
                    vbar.position = APP_MODEL.pointerIndex/APP_MODEL.dataSize
                }
            }

        }

        onHeightChanged: {
            APP_MODEL.makeRecordsModel(height,record_dlg_height)
        }
    }

    Row {
        id: ctrl
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        height: 50
        spacing: 50

        ComboBox {
            id: portsBox
            property string lastPort: ""
            editable: true
            width: 200
            model: APP_MODEL.portList
            onActivated: {
                APP_MODEL.currentPort = currentValue
            }
            Component.onCompleted: {
                APP_MODEL.currentPort = currentValue
            }
        }

        ComboBox {
            id: baudRateBox
            editable: true
            model: ["9600", "115200", "921600"]
            currentIndex: 2
            onActivated: {
                APP_MODEL.baudRate = parseInt(currentValue);
            }
            Component.onCompleted: {
                APP_MODEL.baudRate = parseInt(currentValue);
            }
        }
    }
}
