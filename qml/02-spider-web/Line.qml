import QtQuick 6.0

Item {
  id: root
  antialiasing: true
  property Item item1: null
  property Item item2: null

  property int lineWidth: 2

  Rectangle {
    id: _line
    color: "green"
    antialiasing: root.antialiasing
    //opacity: 0.5
  }

  onItem1Changed: {
    if (!item1)
      return 
    item1.xChanged.connect(function () {internal.updateLine()})
    item1.yChanged.connect(function () {internal.updateLine()})
    internal.updateLine()
  }

  onItem2Changed: {
    if (!item2)
      return
    item2.xChanged.connect(function () {internal.updateLine()})
    item2.yChanged.connect(function () {internal.updateLine()})
    internal.updateLine()
  }



  QtObject {
    id: internal
    // 4 1
    // 3 2
    // 0 for 0
    function quadrant(p, center) {
      const pt = Qt.point(p.x - center.x, p.y - center.y)
      if (pt.x === 0 && pt.y === 0)
        return 0
      
      if (pt.x >= 0)
      {
        if (pt.y >= 0)
          return 1
        else
          return 2
      }
      else
      {
        if (pt.y >= 0)
          return 4
        else
          return 3
      }
      
    }
    function updateLine() {
      if (!item1 || !item2)
        return
      const p1 = Qt.point(item1.x + item1.width / 2, item1.y + item1.height / 2)
      const p2 = Qt.point(item2.x + item2.width / 2, item2.y + item2.height / 2)
      const d = Qt.point(p1.x - p2.x, p1.y - p2.y)
      const dist = Math.sqrt(d.x * d.x + d.y* d.y)
      const cos = Math.abs(d.y) / dist
      const acos = Math.acos(cos)
      const center = Qt.point(p2.x + d.x / 2, p2.y + d.y / 2)
      const rad = 180.0 / Math.PI
      const rot = acos * rad

      _line.width = lineWidth
      _line.height = dist
      _line.x = center.x - _line.width / 2
      _line.y = center.y - _line.height / 2
      _line.rotation = quadrant(p1, center) === 4 || quadrant(p2, center) === 4 ? rot : -rot
    }
  }
}