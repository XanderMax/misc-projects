#include "classifier.h"

#include <QString>
#include <QRandomGenerator>
namespace
{
QString generateRandomString()
{
    const QString characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const int length = 7;

    QString randomString;
    for (int i = 0; i < length; ++i) {
        int randomIndex = QRandomGenerator::global()->bounded(characters.length());
        randomString.append(characters.at(randomIndex));
    }

    return randomString;
}
}

Classifier::Classifier(QObject *parent) : QObject(parent)
{
}

QString Classifier::classify(const QString &imagePath)
{
    return "4";
}

QString Classifier::getPathForImage(const QString& digit)
{
    static const QString basePath = "C:\\Users\\Dmytro.Redko\\hoc\\trainset\\%1\\%2.png";

    return basePath.arg(digit, generateRandomString());
}