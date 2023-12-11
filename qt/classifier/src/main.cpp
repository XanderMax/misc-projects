#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "classifier.h"

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance("ua.mikade", 1, 0, "Classifier", new Classifier());

    const auto qmlPathPattern = QStringLiteral("qrc:///qml/app/%1.qml");
    engine.load(qmlPathPattern.arg(argv[1]));

    app.exec();
}