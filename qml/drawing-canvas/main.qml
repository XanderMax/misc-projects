import QtQuick

Item {
    width: 600
    height: 800
    
    Rectangle {
        id: _background
        width: 282
        height: 282
        anchors.centerIn: _canvas
        color: "white"
        border.width: 1
        border.color: "black"
    }

    Canvas {
        id: _canvas
        width: 28
        height: 28
        smooth: false
        anchors.centerIn: parent
        scale: 10.0
        onPaint: {
            var ctx = getContext("2d")
            ctx.reset()
            ctx.fillStyle = Qt.rgba(.5, .5, .5, 1)
            ctx.moveTo(0, 0)
            ctx.lineWidth = 5
            ctx.lineTo(28, 28)
            ctx.stroke()
            ctx.moveTo(28, 0)
            ctx.lineTo(0, 28)
            ctx.stroke()
        }
    }
}