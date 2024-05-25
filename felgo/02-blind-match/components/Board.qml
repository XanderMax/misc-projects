import QtQuick

Item {
    id: root
    property alias model: _repeater.model
    property int selectedCardIndex: -1
    signal cardClicked(int index)
    signal cardMarked(int index)
    Grid {
        id: _grid
        columns: 4
        columnSpacing: Math.min(root.width * 0.05, root.height * 0.05)
        rows: 5
        rowSpacing: Math.min(root.width * 0.05, root.height * 0.05)
        anchors.fill: parent

        Repeater {
            id: _repeater
            model: 12
            delegate: Card {
                width: (_grid.width - _grid.columnSpacing * (_grid.columns - 1)) / _grid.columns
                height: (_grid.height - _grid.rowSpacing * (_grid.rows - 1)) / _grid.rows
                text: moveableValue + 1
                selected: root.selectedCardIndex === index
                marked: cardMarked
                onLeftClicked: {
                    root.cardClicked(index)
                }
                onRightClicked: {
                    root.cardMarked(index)
                    console.log("Marked: " + index)
                }
            }
        }
    }  
}