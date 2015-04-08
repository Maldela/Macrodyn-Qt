import QtQuick 2.3
import QtQuick.Controls 1.2
import MacrodynQML 1.0


MacrodynGraphicsItem {
    property int x1
    property int x2
    property int y1
    property int y2

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if (mouse.button == Qt.RightButton) {
                parent.unzoom();
            }
            else {
                parent.x1 = parent.x2 = mouse.x;
                parent.y1 = parent.y2 = mouse.y;
                zoomRect.visible = true;
            }
        }
        onPositionChanged: {
            parent.x2 = mouse.x;
            parent.y2 = mouse.y;
        }
        onReleased: {
            if (mouse.button == Qt.LeftButton) {
                zoomRect.visible = false;
                parent.zoom(parent.x1, parent.x2, parent.y1, parent.y2);
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
        y: (parent.y1 < parent.y2) ? parent.y1 : parent.y2;
        width: Math.abs(parent.x1 - parent.x2);
        x: (parent.x1 < parent.x2) ? parent.x1 : parent.x2;
        height: Math.abs(parent.y1 - parent.y2);
        visible: false
    }
}

