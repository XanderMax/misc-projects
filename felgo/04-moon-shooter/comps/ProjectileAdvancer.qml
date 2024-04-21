import QtQuick

Item {
    id: root
    property bool running: true
    property Item projectile
    property real dv: 0
    property real ddv: 0.1

    function reset() {
        self.dv = root.dv
    }

    function advance() {
        if (!projectile || !root.running) return
        self.dv += root.ddv
        projectile.y -= self.dv
    }

    QtObject {
        id: self
        property real dv: 0
    }
}