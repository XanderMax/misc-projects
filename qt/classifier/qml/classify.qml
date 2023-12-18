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
    title: qsTr("Перевір Мене")

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

        DigitSelector {
            id: _digitSelector
            interactive: false
            anchors.left: _boundaries.right
            anchors.right: parent.right
            anchors.top: _buttons.bottom
            height: 500
        }

        Column {
            id: _buttons
            anchors.left: _boundaries.right
            width: 100
            height: 200
            Button {
                width: 120
                height: 100
                text: "Що Намальовано?"
                onClicked: {
                    _digitSelector.select(-1)
                    _timer.start()
                }
            }
            Button {
                width: 120
                height: 100
                text: "Cпочатку"
                onClicked: {
                    _drawingArea.clear()
                    _digitSelector.select(-1)
                }
            }
        }
    }

    Timer {
        id: _timer
        interval: 50
        onTriggered: {
            const filename = Classifier.getPathForTest(_digitSelector.digit)
            console.log("Save to ", filename)
            _drawingArea.save(filename)
            const value = Classifier.classify(filename)
            _digitSelector.select(value)
        }
    }
}