import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0
import MacrodynQML 1.0

ApplicationWindow {

    property int minLogWidth: 200

    id: applicationWindow
    visible: true
    width: 1024
    height: 768
    title: qsTr("Macrodyn-Qt5")

    onWidthChanged: graph.width = Math.min(graph.width, width - minLogWidth)

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Open simulation")
                onTriggered: fileDialog.open();
            }
            MenuItem {
                text: qsTr("Run Simulation")
                onTriggered: loader.runSimulation();
            }
            MenuItem {
                text: "Print parameters"
                onTriggered: loader.printModelParameters();
            }

            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    toolBar:ToolBar {
        RowLayout {
            anchors.topMargin: 0
            anchors.fill: parent
            ToolButton {
                iconSource: "oeffnen.png"
                onClicked: fileDialog.open();
            }
            ToolButton {
                iconSource: "run.png"
                onClicked: loader.runSimulation();
            }
            Item { Layout.fillWidth: true }
//            CheckBox {
//                text: "Enabled"
//                checked: true
//                Layout.alignment: Qt.AlignRight
//            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a simulation"
        modality: Qt.NonModal
        nameFilters: [ "Macrodyn simulation (*.sim)", "All files (*)" ]
        selectedNameFilter: "Macrodyn simulation"
        onAccepted: {
            loader.loadSimulationfromUrl(fileDialog.fileUrl);
            close();
        }
        onRejected: {
            close();
        }
    }

    SimLoader {
        id: loader
        graphItem: graph
    }

    MacrodynGraphicsItem {
        id: graph
        width: 640
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        backgroundColor: "white"

        MouseArea {
           id: mouseAreaRight

           property int oldMouseX

           anchors.horizontalCenter: parent.right
           anchors.top: parent.top
           anchors.bottom: parent.bottom
           width: 10
           hoverEnabled: true
           cursorShape: Qt.SizeHorCursor

           onPressed: {
               oldMouseX = mouseX
           }

           onPositionChanged: {
               if (pressed) {
                   parent.width = Math.min(parent.width + (mouseX - oldMouseX), applicationWindow.width - minLogWidth)
               }
           }
       }
    }
    Log {
        id: log
        anchors.left: graph.right
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        precision: 7
    }
}
