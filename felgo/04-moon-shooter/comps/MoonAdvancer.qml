import QtQuick

Item {
    id: root
    property bool running: true
    property Item moon
    property int dv: 0

    onDvChanged: {
        self.dx = root.dv
        self.dy = root.dv
    }

    onMoonChanged: {
        console.log("Moon changed", moon.width, moon.height, moon.x, moon.y)
        root.reset()
    }

    function intersects(item) {
        if (!item) return false
        const itemPos = item.mapToGlobal(0, 0)
        const moonPos = moon.mapToGlobal(0, 0)
        if (itemPos.x + item.width < moonPos.x || itemPos.x > moonPos.x + moon.width)
            return false
        if (itemPos.y + item.height < moonPos.y || itemPos.y > moonPos.y + moon.height)
            return false
        return true
    }

    function reset() {
        if (!moon) return
        moon.parent = root
        moon.x = root.width / 2 - moon.width / 2
        moon.y = root.height / 2 - moon.height / 2
    }

    function advance() {
        if (!moon || !root.running) return
        const newX = moon.x + self.dx
        const newY = moon.y + self.dy
        if (newX <= 0 || newX + moon.width >= root.width)
            self.dx = -self.dx

        if (newY <= 0 || newY + moon.height >= root.height)
            self.dy = -self.dy

        moon.x = Math.min(Math.max(0, newX), root.width - moon.width)
        moon.y = Math.min(Math.max(0, newY), root.height - moon.height)
    }

    QtObject {
        id: self
        property int dx: 10
        property int dy: 10
    }
}