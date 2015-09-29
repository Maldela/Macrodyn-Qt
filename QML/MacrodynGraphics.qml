import QtQuick 2.3
import QtQuick.Controls 1.2
import MacrodynQML 1.0

MacrodynGraphicsItem {
    property int x1
    property int x2
    property int y1
    property int y2

    backgroundColor: "white"
    supersampling: 2
    bigPointSize: 5

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
