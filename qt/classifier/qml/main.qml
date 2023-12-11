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
    title: qsTr("Тренувальна Вибірка")

    Item {
        width: parent.width
        height: parent.height

        Rectangle {
            id: _boundaries
            color: "black"
            x: 2
            width: Math.min(parent.width - 100, parent.height) - 10
            height: Math.min(parent.width - 100, parent.height) - 10
            DrawingArea {
                id: _drawingArea
                anchors.fill: parent
                anchors.margins: 2
            }
        }

        Column {
            anchors.left: _boundaries.right
            width: 100
            height: parent.height
            Button {
                width: 100
                height: 100
                text: "Зберегти"
                onClicked: {
                    const filename = Classifier.getPathForImage("1")
                    console.log("Save to ", filename)
                    _drawingArea.save(filename)
                    _drawingArea.clear()
                }
            }
            Button {
                width: 100
                height: 100
                text: "Очистити"
                onClicked: {
                    _drawingArea.clear()
                }
            }
        }
    }
}