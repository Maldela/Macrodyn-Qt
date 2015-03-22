import QtQuick 2.3
import QtQuick.Controls 1.2
import MacrodynQML 1.0

TextArea {
    property int precision: 7

    signal error

    id: textarea
    readOnly: true

<<<<<<< HEAD
//    font.family: "Arial"
//    font.pointSize: 10
=======
    font.family: "Arial"
    font.pointSize: 12
>>>>>>> origin/master

    Logger {
        property bool error: false

        precision: textarea.precision
        onLineChanged: {
            textarea.append(line);
        }
        onErrorOccured: {
            parent.error();
        }
        onFatalErrorOccured: {
            parent.error();
        }
    }
}
