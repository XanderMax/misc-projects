#include "classifier.h"

#include <QString>
#include <QRandomGenerator>
#include <QProcess>
#include <QFile>
#include <QTextStream>
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

int readTextFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Handle file open error
        return -1;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    file.close();

    bool ok;
    int value = text.toInt(&ok);
    if (!ok)
    {
        // Handle conversion error
        return -1;
    }

    return value;
}

} // namespace

Classifier::Classifier(QObject *parent) : QObject(parent)
{
}

int Classifier::classify(const QString &imagePath)
{
    static const QString modelPath = QStringLiteral("C:\\Users\\Dmytro.Redko\\hoc\\model.keras");
    static const QString classifyScript = QStringLiteral("C:\\Users\\Dmytro.Redko\\dev\\projects\\misc-projects\\py\\classifier\\classify.py");
    const QString outputFilePattern = QStringLiteral("C:\\Users\\Dmytro.Redko\\hoc\\output\\%1.txt");
    const auto outputFile = outputFilePattern.arg(generateRandomString());
    const QString program = "python.exe";
    const QStringList arguments {classifyScript, "--model", modelPath, "--image", imagePath, "--output", outputFile};
    QProcess::execute(program, arguments);
    
    return readTextFromFile(outputFile);
}

QString Classifier::getPathForImage(const QString& digit)
{
    static const QString basePath = "C:\\Users\\Dmytro.Redko\\hoc\\trainset\\%1\\%2.png";

    return basePath.arg(digit, generateRandomString());
}

QString Classifier::getPathForTest()
{
    static const QString basePath = "C:\\Users\\Dmytro.Redko\\hoc\\output\\%1.png";
    return basePath.arg(generateRandomString());
}

