import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0
import MacrodynQML 1.0

ApplicationWindow {

    property int minLogWidth: 200
    property int minEditorWidth: 200
    property int minGraphWidth: 100

    id: applicationWindow
    visible: true
    width: 1424
    height: 768
    title: qsTr("Macrodyn-Qt5")

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
                iconSource: "oeffnen.png"
                onClicked: fileDialogOpen.open();
            }
            ToolButton {
                iconSource: "speichern.png"
                onClicked: loader.saveSimulationfromFile(); //fileDialogSave.open();
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
        id: fileDialogOpen
        title: "Please choose a simulation"
        modality: Qt.NonModal
        nameFilters: [ "Macrodyn simulation (*.sim)", "All files (*)" ]
        selectedNameFilter: "Macrodyn simulation"
        onAccepted: {
            loader.loadSimulationfromUrl(fileDialogOpen.fileUrl);
            loader.fileUrl = fileUrl;
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
            graph.savePdf(fileDialogSavePdf.fileUrl);
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
            loader.saveSimulationfromUrl(fileDialogSave.fileUrl);
            loader.fileUrl = fileUrl;
            close();
        }
        onRejected: {
            close();
        }
    }

    SimLoader {
        id: loader
        graphItem: graph
        target: simeditor
    }

    MacrodynGraphicsItem {
        property int x1
        property int x2
        property int y1
        property int y2

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

//        onWidthChanged: if (width < minGraphWidth) width = minGraphWidth;

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: {
                if (mouse.button == Qt.RightButton) {
                    graph.unzoom();
                }
                else {
                    graph.x1 = graph.x2 = mouse.x;
                    graph.y1 = graph.y2 = mouse.y;
                    zoomRect.visible = true;
                }
            }
            onPositionChanged: {
                graph.x2 = mouse.x;
                graph.y2 = mouse.y;
            }
            onReleased: {
                if (mouse.button == Qt.LeftButton) {
                    zoomRect.visible = false;
                    graph.zoom(graph.x1, graph.x2, graph.y1, graph.y2);
                }
            }
        }

        BusyIndicator {
            id: busyIndicator
            running: parent.redrawing
            anchors.centerIn: parent
        }

        Rectangle {
            id: zoomRect
            color: "transparent"
            border.color: "black"
            border.width: 1
            y: (graph.y1 < graph.y2) ? graph.y1 : graph.y2;
            width: Math.abs(graph.x1 - graph.x2);
            x: (graph.x1 < graph.x2) ? graph.x1 : graph.x2;
            height: Math.abs(graph.y1 - graph.y2);
            visible: false
        }
    }


    TextArea{
        id: simeditor
        width: 400
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: log.left
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        focus: true
        text: loader.text

        wrapMode: TextEdit.Wrap

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


