import QtQuick

Item {
    property alias color: _rect.color

    Rectangle {
        id: _shadow
        color: "black"
        opacity: 0.2

        width: _rect.width * 1.1
        height: _rect.height * 1.1
    }

    Rectangle {
        id: _rect
        color: "red"
        anchors.fill: parent
    }

}