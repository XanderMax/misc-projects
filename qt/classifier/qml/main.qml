import QtQuick
import QtQuick.Window
import ua.mikade
import "."

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Component.onCompleted: {
        console.log("Hello from QML", Classifier.getPathForImage("1"))
    }

    Item {
        width: parent.width
        height: parent.height

        DrawingArea {
            anchors.fill: parent
        }
    }
}