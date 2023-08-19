import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("SerialPort Reader")

    Text {
        width: parent.width
        anchors.bottom: ctrl.top
        anchors.top: parent.top
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: APP_MODEL.serialData
        padding: 2
    }

    Row {
        id: ctrl
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
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
