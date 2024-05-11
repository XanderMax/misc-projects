import QtQuick

Item {
    id: root
    function print_name_and_age ({name = "", age = 0} = {}) {
        console.log(name + " " + age)
    }
    Component.onCompleted: {
        root.print_name_and_age({name: "Tom", age: 12})
    }
}