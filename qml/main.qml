import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("SerialPort Reader")

    ListView {
        id: dataList
        width: parent.width
        model: APP_MODEL.serialData
        anchors {
            top: parent.top
            topMargin: 10
            bottom: ctrl.top
            bottomMargin: 10
        }
        delegate: Item {
            width: dataList.width
            height: 20
            Text {
                anchors.fill: parent
                text: modelData
                padding: 2
            }
        }
        onModelChanged: {
            positionViewAtEnd()
        }
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
