import QtQuick
import QtQuick.Window
import ua.mikade
import QtQuick.Controls
import QtQuick.Layouts
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
            id: _drawingArea
            width: 300
            height: 300
            anchors.centerIn: parent
        }

        Column {
            anchors.left: _drawingArea.right
            width: 100
            height: parent.height
            Button {
                width: 100
                height: 100
                text: "Save"
                onClicked: {
                    const filename = Classifier.getPathForImage("1")
                    console.log("Save to ", filename)
                    _drawingArea.save(filename, Qt.size(28, 28))
                    _drawingArea.clear()
                }
            }
            Button {
                width: 100
                height: 100
                text: "Clear"
                onClicked: {
                    _drawingArea.clear()
                }
            }
        }

    }
}