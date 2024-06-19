import QtQuick

Item {
    id: root
    property alias text: _text.text
    property bool selected: false
    property bool marked: false
    signal leftClicked()
    signal rightClicked()

    Rectangle {
        anchors.centerIn: parent
        color: root.selected ? "#f54e42" : "#000000"
        border.color: root.marked ? "#4dd163" : "#ffffff"
        border.width: 3
        width: internal.width > root.width ? root.width : internal.width
        height: internal.height > root.height ? root.height : internal.height

        Text {
            id: _text
            anchors.centerIn: parent
            width: parent.width * 0.75
            height: parent.height * 0.75
            fontSizeMode: Text.Fit
            font.pixelSize: 72
            font.bold: true
            color: "#ffffff"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: (mouse) => {
                if (mouse.button === Qt.LeftButton)
                    root.leftClicked()
                if (mouse.button === Qt.RightButton)
                    root.rightClicked()
            }
            onPressAndHold: {
                root.rightClicked()
            }
        }
    }

    QtObject {
        id: internal
        readonly property real whRatio: 0.75
        readonly property real width: root.height * whRatio
        readonly property real height: root.width / whRatio
    }
}