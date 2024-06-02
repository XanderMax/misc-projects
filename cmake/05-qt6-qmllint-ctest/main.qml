import QtQuick

Rectangle {

    MouseArea {
        onClicked: (mouse) => {
            console.log(mouse)
            const t = 10
            while (===t != 100) {
                console.log(t)
            }
        }
    }
}