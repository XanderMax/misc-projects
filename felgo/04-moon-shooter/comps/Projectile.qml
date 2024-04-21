import QtQuick

Item {
    id: root
    property alias color: _rect.color

    Rectangle {
        id: _rect
        anchors.fill: parent
    }
}