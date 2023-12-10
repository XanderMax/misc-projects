import QtQuick

Item {
    
    function save(filename) {
        _canvas.save(filename)
    }
    function clear() {
        _canvas.paths = []
        _canvas.currentPath = []
        _canvas.requestPaint()
    }
    Rectangle {
        color: "white"
        border.color: "black"
        border.width: 1
        width: 282
        height: 282
        anchors.centerIn: parent

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onPositionChanged: (mouse) => {
                if (pressed) {
                    var xx = mouse.x / width
                    var yy = mouse.y / height
                    _canvas.currentPath.push(Qt.point(xx, yy))
                    _canvas.requestPaint()
                }
            }
            onReleased: {
                console.log("Released")
                _canvas.paths.push(_canvas.currentPath)
                _canvas.currentPath = []
                _canvas.requestPaint()
            }
        }
    }

    Canvas {
        id: _canvas
        property var paths: [[Qt.point(0, 0), Qt.point(0.1, 0.1)]]
        property var currentPath: []
        width: 28
        height: 28
        smooth: false
        anchors.centerIn: parent
        scale: 10.0
        onPaint: {
            var ctx = getContext("2d")
            ctx.reset()
            ctx.fillStyle = Qt.rgba(1, 1, 1, 1)
            ctx.fillRect(0, 0, _canvas.width, _canvas.height);
            ctx.fillStyle = Qt.rgba(0, 0, 0, 1)
            ctx.lineWidth = 2
            for (const index in paths) {
                drawPath(ctx, paths[index])
            }
            if (currentPath.length >= 2)
                drawPath(ctx, currentPath)
            ctx.stroke()
        }

        function drawPath(ctx, points) {
            if (points.length < 2) return
            var pt = points[0]
            ctx.moveTo(pt.x * width, pt.y * height)
            for (var i = 1; i < points.length; ++i) {
                var p = points[i]
                var absP = Qt.point(p.x * width, p.y * height)
                ctx.lineTo(absP.x, absP.y)
                ctx.moveTo(absP.x, absP.y)
            }
        }
    }
}