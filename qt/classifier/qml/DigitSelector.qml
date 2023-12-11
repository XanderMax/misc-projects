import QtQuick

Item {
    id: root
    property bool interactive: true
    readonly property bool selected: internal.selected !== -1
    readonly property string digit: internal.selected !== -1 ? internal.selected.toString() : ""

    function select(index) {
        if (index < 0 || index >= _model.count) {
            return
        }
        internal.selected = index
    }

    Grid {
        columns: 2
        rows: 5
        columnSpacing: 5
        rowSpacing: 5
        anchors.fill: parent

        Repeater {
            id: _repeater
            anchors.fill: parent
            model: _model
            Rectangle {
                property bool selected: internal.selected === index
                width: 50
                height: 50
                color: "yellow"
                border.color: selected ? "red" : "gray"
                border.width: 5
                
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 72
                    color: parent.selected ? "black" : "gray"
                    text: index
                    anchors.fill: parent
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    enabled: root.interactive
                    onClicked: {
                        console.log("clicked", index)
                        root.select(index)
                    }
                
                }
            }
        }
    }

    ListModel {
        id: _model

        ListElement {
            selected: false
        }
        ListElement {
            selected: false
        }
        ListElement {
            selected: false
        }
        ListElement {
            selected: false
        }
        ListElement {
            selected: false
        }
        ListElement {
            selected: false
        }
        ListElement {
            selected: false
        }
        ListElement {
            selected: false
        }
        ListElement {
            selected: false
        }
        ListElement {
            selected: false
        }
    }

    QtObject {
        id: internal
        property int selected: -1
    }
}