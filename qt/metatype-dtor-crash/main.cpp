#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include "types.h"

const char* QML_DATA = 
"import QtQuick 6.3\n"
"import QtQuick.Window 2.11\n"
"import my.types 1.0\n"
"Window {\n"
"   id: root\n"
"   width: 1000\n"
"   height: 800\n"
"   visible: true \n"
"   Component.onCompleted: {\n"
"       const g = MySingleton.get()\n"
"   }\n"
"}\n"
;

int main(int argc, char** argv)
{   
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    qmlRegisterSingletonInstance("my.types", 1, 0, "MySingleton", new MySingleton(&app));
    engine.loadData(QByteArray{QML_DATA, -1}, QUrl{});
    app.exec();
}