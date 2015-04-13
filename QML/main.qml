import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0

ApplicationWindow {

    property int minLogWidth: 200
    property int minEditorWidth: 200
    property int minGraphWidth: 100

    id: applicationWindow
    visible: true
    width: 1424
    height: 768
    title: qsTr("MacroDyn")

    onWidthChanged: graph.width = Math.min(graph.width, width - minLogWidth)

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Open simulation")
                onTriggered: fileDialogOpen.open();
            }
            MenuItem {
                text: qsTr("Save simulation")
                onTriggered: fileDialogSave.open();
            }
            MenuItem {
                text: qsTr("Run Simulation")
                onTriggered: loader.runSimulation();
            }
            MenuItem {
                text: "Print Graph"
                onTriggered: graph.print();
            }
            MenuItem {
                text: "Print parameters"
                onTriggered: loader.printModelParameters();
            }
            MenuItem {
                text: "Save pdf"
                onTriggered: loader.savePdf();
            }

            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    toolBar: ToolBar {
        RowLayout {
            anchors.topMargin: 0
            anchors.fill: parent
            ToolButton {
                iconSource: "/Icons/oeffnen.png"
                onClicked: fileDialogOpen.open();
            }
            ToolButton {
                iconSource: "/Icons/speichern.png"
                onClicked: loader.saveSimulationToFile(); //fileDialogSave.open();
            }
            ToolButton {
                iconSource: "/Icons/pdfspeichern.png"
                onClicked:  loader.savePdf();
            }
            ToolButton {
                iconSource: "/Icons/run.png"
                onClicked: loader.runSimulation();
            }
            Image {
                source: "/Icons/Logo.png"
                anchors {
                    right: parent.right
                }
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
        id: fileDialogOpen
        title: "Please choose a simulation"
        modality: Qt.NonModal
        nameFilters: [ "Macrodyn simulation (*.sim)", "All files (*)" ]
        selectedNameFilter: "Macrodyn simulation"
        onAccepted: {
            loader.loadSimulationfromUrl(fileUrl);
            close();
        }
        onRejected: {
            close();
        }
    }

    FileDialog {
        id: fileDialogSavePdf
        title: "Save as PDF"
        modality: Qt.NonModal
        selectMultiple: false
        selectExisting: false
        //accepted: QFileDialog.AcceptSave
        nameFilters: [ "PDF (*.pdf)", "All files (*)" ]
        selectedNameFilter: "PDF"
        onAccepted: {
            graph.savePdf(fileUrl);
            close();
        }
        onRejected: {
            close();
        }
    }

    FileDialog {
        id: fileDialogSave
        title: "Please choose a simulation"
        modality: Qt.NonModal
        nameFilters: [ "Macrodyn simulation (*.sim)", "All files (*)" ]
        selectedNameFilter: "Macrodyn simulation"
        onAccepted: {
            loader.saveSimulationToUrl(fileUrl);
            close();
        }
        onRejected: {
            close();
        }
    }

    MacrodynGraphicsItem {
        id: graph
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: simeditor.left
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        backgroundColor: "white"
        supersampling: 2
        bigPointSize: 5

        Component.onCompleted: loader.setGraphItem(this);
    }

    TextArea {
        id: simeditor
        width: 400
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: log.left
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        //focus: true
        text: loader.text
        font.family: "Arial"
        font.pointSize: 12

        onTextChanged: loader.text = text;
        onWidthChanged: if (width < minEditorWidth) width = minEditorWidth;
    }

    Log {
        id: log

        //anchors.left: simeditor.right
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        width: 400
        precision: 7

        onWidthChanged: if (width < minLogWidth) width = minLogWidth;
    }

    MouseArea {
       id: mouseAreaRight

       property int oldMouseX
       property int oldWidth

       anchors.horizontalCenter: simeditor.right
       anchors.top: simeditor.top
       anchors.bottom: simeditor.bottom
       width: 10
       hoverEnabled: true
       cursorShape: Qt.SizeHorCursor

       onPressed: {
           oldMouseX = mouseX;
           oldWidth = log.width + simeditor.width;
       }

       onPositionChanged: {
           if (pressed) {
               log.width -= mouseX - oldMouseX;
               simeditor.width = oldWidth - log.width;
           }
       }
    }

    MouseArea {
       id: mouseAreaLeft

       property int oldMouseX

       anchors.horizontalCenter: graph.right
       anchors.top: graph.top
       anchors.bottom: graph.bottom
       width: 10
       hoverEnabled: true
       cursorShape: Qt.SizeHorCursor

       onPressed: {
           oldMouseX = mouseX
       }

       onPositionChanged: {
           if (pressed) {
               simeditor.width -= mouseX - oldMouseX;
           }
       }
    }
}


