import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

ApplicationWindow  {
    width: 640
    height: 480
    visible: true
    title: qsTr("SerialPort Reader")

//    ScrollView {
//        contentWidth: -1
//        width: parent.width
//        height: ctrl.y
//        Text {
//            width: parent.width
//            height: ctrl.y
//    //        verticalAlignment: Text.AlignVCenter
//    //        horizontalAlignment: Text.AlignHCenter
//            text: APP_MODEL.serialData
//            wrapMode: Text.Wrap
//            padding: 2
//        }
//    }

    ListView {
        id: contentList
        width: parent.width - 10
        height: ctrl.y - 10
        anchors.horizontalCenter: parent.horizontalCenter
        model: APP_MODEL.recordList
        delegate: Item {
            id: dlg
            width: contentList.width
            height: 20
            Text {
                anchors.fill: parent
                text: dlg.visible? modelData.raw : ""
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
        ScrollBar.vertical: ScrollBar { }
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

        /*
        Button {
            id: startStop
            width: 100
            height: 40
            text: "Start"
            onClicked: {
                APP_CTRL.startReadPort(portsBox.currentValue)
            }
        }
        */
    }
}
