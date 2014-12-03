import QtQuick 2.0
import QtQuick.Controls 1.2
import MacrodynQML 1.0

TextArea {
    property int precision: 6

    readOnly: true

    Logger {
        precision: parent.precision
        onLineChanged: parent.append(line)
    }
}
