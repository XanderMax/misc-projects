import QtQuick 6.0

import "."

Item {
    id: root

    property size dimension
    property Component pointDelegate

    onDimensionChanged: {
        internal.update()
    }

    QtObject {
        id: internal
        property list<Item> points
        property list<Item> lines

        function func(val) {
            return function(){return val}
        }

        function update() {
            if (!root.pointDelegate)
                return
            lines = []
            points = []

            const rowsCount = root.dimension.height
            const colsCount = root.dimension.width
            const stepInCol = 1. / colsCount
            const stepInRow = 1. / rowsCount

            var counter = 0
            for (var row = 0; row < rowsCount; ++row) {
                for (var col = 0; col < colsCount; ++col) {
                    const coords = {
                        "xx": stepInCol * col,
                        "yy": stepInRow * row,
                        "index": counter++,
                        "z": 1
                    }
                    const item = pointDelegate.createObject(root, coords)
                    if (!item) {
                        console.log("WTF")
                        return
                    }
                    points.push(item)
                }
            }

            const lineComponent = Qt.createComponent("Line.qml");
            if (lineComponent.status !== Component.Ready) {
                console.log("WTF")
                return
            }
            for (var col = 0; col < colsCount; ++col) {
                for (var row = 0; row < rowsCount - 1; ++row) {

                    const topIndex = row * colsCount + col
                    const bottomIndex = (row + 1) * colsCount + col
                    
                    const topItem = points[topIndex]
                    const bottomItem = points[bottomIndex]
                    
                    const lineItem = lineComponent.createObject(root, {"item1": topItem, "item2": bottomItem, "z": 0})
                    if (!lineItem) {
                        console.log("WTF")
                        return
                    }
                    lines.push(lineItem)
                }
            }
            for (var row = 0; row < rowsCount; ++row) {
                for (var col = 0; col < colsCount - 1; ++col) {

                    const leftIndex = row * colsCount + col
                    const rightIndex = row * colsCount + col + 1
                    
                    const leftItem = points[leftIndex]
                    const rightItem = points[rightIndex]

                    const lineItem = lineComponent.createObject(root, {"item1": leftItem, "item2": rightItem, "z": 0})
                    if (!lineItem) {
                        console.log("WTF")
                        return
                    }
                    lines.push(lineItem)
                }
            }
        }
    }
}