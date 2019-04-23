import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

Window {
    visible: true
    width: 400
    height: 480
    title: qsTr("Client CAN")

    property bool connected : false

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Button {
                text: "Rafraîchir"
                onClicked: {
                    busCanInterface.discoverPeakDevices()
                }
            }
            ComboBox {
                id: deviceListCombo
                model: busCanInterface.devicesList
            }
            Button {
                text: "Se connecter"
                enabled : deviceListCombo.currentText !== ""
                onClicked: {
                    connected = busCanInterface.init("peakcan", deviceListCombo.currentText)
                }
            }
            StatusIndicator {
                active: connected
                color: "green"
            }
        }

        RowLayout {

            Text {
                text: qsTr("Streaming mode")
            }
            Button {
                text: "Switch"
                onClicked: {
                    busCanInterface.swithStreamingMode()
                }
            }

            Text {
                text: busCanInterface.streamingMode?"Streaming actif":"Streaming inactif"
            }
        }
    }
}
