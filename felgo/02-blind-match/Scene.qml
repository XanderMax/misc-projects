import QtQuick
import "./components"

Rectangle {
    id: root
    color: "#000000"
    Component.onCompleted: {
        const count = 8
        const array = new Array()
        for (let i = 0; i < count; ++i) {
            array.push(i)
        }
        const derangedArray = internal.randomDerangement(array)
        console.log("derangedArray", derangedArray)
        for (let i = 0; i < count; ++i) {
            _boardModel.append({moveableValue: i, staticValue: derangedArray[i], cardMarked: false})
        }
        internal.matches = internal.countMatches()
    }

    Text {
        anchors.top: parent.top
        anchors.bottom: _board.top
        width: parent.width
        text: internal.matches
        font.pixelSize: 72
        fontSizeMode: Text.Fit
        font.bold: true
        color: "#ffffff"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Board {
        id: _board
        width: root.width * 0.9
        height: root.height * 0.6
        anchors.centerIn: root
        model: _boardModel
        onCardClicked: (cardIndex) => {
            if (cardIndex < 0 || cardIndex >= _boardModel.count) {
                return;
            }
            if (_board.selectedCardIndex === -1) {
                _board.selectedCardIndex = cardIndex
                return;
            }
            const data0 = _boardModel.get(_board.selectedCardIndex)
            const data1 = _boardModel.get(cardIndex)
            const data0DV = (data0.moveableValue + 1) - 1
            const data1DV = (data1.moveableValue + 1) - 1
            _boardModel.setProperty(_board.selectedCardIndex, "moveableValue", data1DV)
            _boardModel.setProperty(cardIndex, "moveableValue", data0DV)
            _board.selectedCardIndex = -1
            internal.matches = internal.countMatches()
        }
        onCardMarked: (cardIndex) => {
            const cardMarked = _boardModel.get(cardIndex).cardMarked
            _boardModel.setProperty(cardIndex, "cardMarked", !cardMarked)
        }
    }

    ListModel {
        id: _boardModel
    }

    QtObject {
        id: internal
        property int matches: 0
        function randomItemFromSet(set) {
            const items = Array.from(set);
            return items[Math.floor(Math.random() * items.length)];
        }

        function countMatches() {
            let count = 0;
            for (let i = 0; i < _boardModel.count; ++i) {
                const data = _boardModel.get(i)
                if (data.moveableValue === data.staticValue) {
                    count++
                }
            }
            return count
        }

        function randomDerangement(array) {
            const derangedArray = Array.from(array);
            let currentIndex = derangedArray.length;

            while (currentIndex !== 0) {
                const randomIndex = Math.floor(Math.random() * currentIndex);
                currentIndex--;

                [derangedArray[currentIndex], derangedArray[randomIndex]] = [derangedArray[randomIndex], derangedArray[currentIndex]];
            }

            return derangedArray;
        }
    }
}


