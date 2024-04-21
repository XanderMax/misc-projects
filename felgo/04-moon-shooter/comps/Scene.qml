import QtQuick 

Item {
    id: root
    MoonAdvancer {
        id: _moonAdvancer
        height: root.height * 0.85
        width: root.width
        dv: 5

        moon: Moon {
            id: _moon
            height: Math.min(_moonAdvancer.height, _moonAdvancer.width) * 0.15
            width: height
            color: "yellow"
        }
    }

    ProjectileAdvancer {
        id: _projectileAdvancer
        running: false
        dv: 10
        projectile: _projectile
    }

    Projectile {
        id: _projectile
        height: 30
        width: 10
        color: "red"
        x: root.width / 2 - _projectile.width / 2
        y: root.height - _projectile.height
    }

    Timer {
        id: _timer
        repeat: true
        interval: 16
        running: false
        onTriggered: {
            _moonAdvancer.advance()
            _projectileAdvancer.advance()
            if (_moonAdvancer.intersects(_projectile)) {
                console.log("hit")
                _timer.running = false
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        onMouseXChanged:  {
            if (pressed)
                _projectile.x = mouseX - _projectile.width / 2
        }
        onReleased: {
            _projectileAdvancer.running = true
        }
        onPressed: {
            if (!_timer.running) {
                _moonAdvancer.reset()
                _timer.running = true
            }
            _projectileAdvancer.reset()
            _projectileAdvancer.running = false
            _projectile.x = root.width / 2 - _projectile.width / 2
            _projectile.y = root.height - _projectile.height
        }
    }
}