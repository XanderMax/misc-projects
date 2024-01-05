import QtQuick
import QtQml.Models
import Felgo

App {

    Rectangle {
        id: root
        color: "black"
        anchors.fill: parent

        focus: true

        Rectangle {
            id: _window
            color: "black"
            anchors.centerIn: parent
            width: internal.width
            height: internal.height
        }

        Rectangle {
            id: _water
            color: "blue"
            anchors.left: _window.left
            anchors.right: _window.right
            anchors.bottom: _window.bottom
        }

        Component {
            id: _rectComp
            Rectangle {
                readonly property int line: 0
                readonly property real accelation: 0.05
                property real velocity: 0.0
                readonly property real maxV: 10.0
                x: 0
                y: -width
                width: _window.width / 8
                height: width
            }
        }
        
        Timer {
            id: _timer
            property real drainRate: 0.4
            property bool rain: false
            property real rainRate: 0
            property int counter: 0
            interval: 5
            running: true
            repeat: true
            onTriggered: {
                var killList = []
                for (var idx in _window.children) {
                    var item =  _window.children[idx];
                    item.velocity = Math.min(item.velocity + item.accelation, item.maxV)
                    item.y += item.velocity
                    if (item.y >= _water.y) {
                        killList.push(item)
                    }
                }
                for (var idx in killList) {
                    killList[idx].destroy()
                }
                if (!rain) _water.height = Math.min(Math.max(_water.height - drainRate, 0), _window.height)
                if (rain && ++counter >= rainRate) {
                    internal.addItem()
                    counter = 0
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                _timer.rain ^= true
            }
        }

        QtObject {
            id: internal
            property var lines: [[],[],[],[],[],[],[]]
            readonly property real ratio: root.width / root.height
            readonly property real defRatio: defW / defH
            readonly property int width: ratio >= defRatio ? root.width * (defRatio / ratio) : root.width
            readonly property int height: ratio >= defRatio ? root.height : root.height * (ratio / defRatio)
            readonly property int defW: 600
            readonly property int defH: 1000

            function addItem() {
                var line = Math.floor(Math.random() * 25)
                var isBlack = Math.floor(Math.random() * 2) == 0
                var item = _rectComp.createObject(_window, {line: line,
                color: isBlack ? "#0546f7" : "#4071f7"});
                item.width = _window.width / 25
                item.x = line * item.width
                item.y = -500
            }
        }
    }
}