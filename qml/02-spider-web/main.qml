import QtQuick 6.0

Rectangle {
    id: root
    color: "white"
    width: 1000
    height: 800

    Web {
        id: _web
        anchors.fill: parent
        dimension: Qt.size(100, 64)
        pointDelegate: Rectangle {
            property real xx
            property real yy
            property int index: 1
            color: "red"
            width: 5
            height: 5
            x: xx * root.width
            y: yy * root.height
            MouseArea {
                anchors.fill: parent
                drag.target: parent
                drag.axis: Drag.XAndYAxis
            }
        }
    }
}