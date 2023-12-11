import QtQuick
import QtQuick.Window
import ua.mikade

Window {
    id: root
    width: 800
    height: 700
    visible: true
    property var files: Classifier.filesInTestset()
    title: qsTr("Протестуй Мене")

    MouseArea {
        anchors.fill: parent
        onClicked: {
            enabled = false
            _timer.start()
            _column.visible = Qt.binding(function() {return !_timer.running})

        }
    }

    Image {
        id: _image
        anchors.centerIn: parent
        width: 280
        height: 280
    }

    Timer {
        id: _timer
        repeat: true
        interval: 150
        running: false
        onTriggered: {
            if (files.length === 0) {
                stop()
                _image.visible = false
                return
            }
            const file = files.pop()
            console.log(file)
            _image.source = file
        }
    }

    Column {
        id: _column
        visible: false
        width: 700
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.centerIn: parent
        spacing: 5
        Repeater {
            anchors.fill: parent
            model: _model
            Rectangle {
                height: 50
                color: "#4287f5"
                width: parent.width * percentage

                Text {
                    anchors.left: parent.left
                    width: parent.width
                    height: parent.height
                    fontSizeMode: Text.Fit
                    font.pixelSize: 72
                    text: index + " : " + (percentage * 100).toFixed(0) + "%"
                }
            }
        }
    }

    ListModel {
        id: _model
        ListElement {percentage: 0.43}
        ListElement {percentage: 0.44}
        ListElement {percentage: 0.50}
        ListElement {percentage: 0.63}
        ListElement {percentage: 0.57}
        ListElement {percentage: 0.69}
        ListElement {percentage: 0.34}
        ListElement {percentage: 0.56}
        ListElement {percentage: 0.39}
        ListElement {percentage: 0.47}
    }
}