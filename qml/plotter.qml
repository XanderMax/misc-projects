import QtQuick 6.0

//~/Qt/6.3.0/gcc_64/bin/qml plotter.qml 2>&1 | awk '{print $3}'

Item {
    width: 1000
    height: 800

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onPressed: {
            var p = Qt.point(mouseX / width, mouseY / height)
            internal.lastPoint = p
            internal.leftWall = p
            internal.floor = p
            internal.points = [p]
            internal.rightWall = undefined
            internal.ceil = undefined
            internal.curCeil = undefined
            _canvas.requestPaint()
        }
        onReleased: {
            var w = Math.abs(internal.lastPoint.x - internal.floor.x)
            var h = Math.abs(internal.lastPoint.y - internal.floor.y)
            for (var idx in internal.points) {
                var p = internal.points[idx]
                var pt = Qt.point(p.x - internal.floor.x, p.y - internal.lastPoint.y)
                console.log(pt.x / w, "\t", 1 - pt.y / h)
            }
            // internal.points.push(Qt.point(1,1))
            // _canvas.requestPaint()
        }
        onPositionChanged: {
            if (!pressed)
                return
            var p = Qt.point(mouseX / width, mouseY / height)
            if (internal.lastPoint) {
                var dx = internal.lastPoint.x - p.x
                var dy = internal.lastPoint.y - p.y
                var dist = Math.sqrt(dx * dx + dy * dy)
                if (dist < 0.005)
                    return
            }
            internal.ceil = !internal.ceil || internal.ceil.y > p.y ? p : internal.ceil
            internal.curCeil = p
            internal.lastPoint = p;
            internal.rightWall = p
            internal.points.push(internal.lastPoint)
            _canvas.requestPaint()
        }
    }

    Canvas {
        id: _canvas
        anchors.fill: parent
        
        onPaint: {
            if (internal.points.length === 0)
                return
            var ctx = getContext("2d")
            ctx.reset()
            ctx.fillStyle = Qt.rgba(.5, .5, .5, 1)
            ctx.lineWidth = 2
            drawHLine(ctx, internal.floor)
            drawVLine(ctx, internal.leftWall)
            drawVLine(ctx, internal.rightWall)
            drawHLine(ctx, internal.ceil)
            drawHLine(ctx, internal.curCeil)
            var pt = internal.points[0]
            ctx.moveTo(pt.x * width, pt.y * height)
            for (var i = 1; i < internal.points.length; ++i) {
                var p = internal.points[i]
                var absP = Qt.point(p.x * width, p.y * height)
                ctx.lineTo(absP.x, absP.y)
                ctx.moveTo(absP.x, absP.y)
            }
            ctx.stroke()
        }

        function drawHLine(ctx, p) {
            if (!ctx || !p)
                return
            ctx.moveTo(0, p.y * height)
            ctx.lineTo(width, p.y * height)
        }

        function drawVLine(ctx, p) {
            if (!ctx || !p)
                return
            ctx.moveTo(p.x * width, 0)
            ctx.lineTo(p.x * width, height)
        }
    }

    QtObject {
        id: internal
        property var points: []
        property var floor
        property var leftWall
        property var rightWall
        property var ceil
        property var curCeil
        property var lastPoint
    }
}