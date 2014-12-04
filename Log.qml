import QtQuick 2.3
import QtQuick.Controls 1.2
import MacrodynQML 1.0

TextArea {
    property int precision: 7

    id: textarea
    readOnly: true
//    text: "New Log"

    Logger {
        precision: textarea.precision
        onLineChanged: {
            textarea.append(line);
        }
    }
}
