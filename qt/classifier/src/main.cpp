#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "classifier.h"

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance("ua.mikade", 1, 0, "Classifier", new Classifier());

    engine.load("qrc:///qml/app/main.qml");

    app.exec();
}