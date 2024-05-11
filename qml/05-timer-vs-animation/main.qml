import QtQuick

Item {
    id: root
    height: 700
    width: 600

    Rectangle {
        id: _rect1
        height: 50
        width: 50
        color: "red"
    }

    Rectangle {
        id: _rect2
        height: 50
        width: 50
        color: "green"
        x: parent.width - _rect2.width
    }

    Timer {
        id: _timer
        property int dy: 0
        interval: 30
        running: true
        repeat: true
        onTriggered: {
            const dv = 10
            if (_rect1.y === 0)
                dy = dv
            else if (_rect1.y === parent.height - _rect1.height)
                dy = -dv

            _rect1.y += dy     
        }
    }

    NumberAnimation {
        id: _anim
        target: _rect2
        property: "y"
        from: 0
        to: root.height - _rect2.height
        duration: 2000
        running: true
        onFinished: {
            if (_rect2.y === 0) {
                _anim.from = 0
                _anim.to = root.height - _rect2.height
            } else if (_rect2.y === root.height - _rect2.height) {
                _anim.from = root.height - _rect2.height
                _anim.to = 0
            }
            restart()
        }
    }
}