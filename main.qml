import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4

Window {
    id: control
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

        Rectangle {
            border.width: 1
            height: 2
            width: control.width
            anchors.margins: 20
            border.color: "#2d2b19"
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
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
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            visible : busCanInterface.streamingMode
            Text {
                text: qsTr("SAMA RX Speed")
            }
            Text {
                Layout.fillWidth: true
                text: Number(busCanInterface.serverRXSpeed)
            }
        }

        Rectangle {
            border.width: 1
            height: 2
            width: control.width
            anchors.margins: 20
            border.color: "#2d2b19"
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Text {
                text: "Couleur bleue : "
            }
            Switch {
                id: blueSwitch
                onClicked: {
                    busCanInterface.setLedBlue(blueSwitch.checked)
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Text {
                text: "Couleur verte : "
            }
            Switch {
                id: greenSwitch
                onClicked: {
                    busCanInterface.setLedGreen(greenSwitch.checked)
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Text {
                text: "Couleur rouge : "
            }
            Switch {
                id: redSwitch
                onClicked: {
                    busCanInterface.setLedRed(redSwitch.checked)
                }
            }
        }
    }
}
