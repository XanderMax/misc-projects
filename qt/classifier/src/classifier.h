#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

class Classifier : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_NAMED_ELEMENT(Classifier)
public:
    explicit Classifier(QObject *parent = nullptr);

public slots:
    int classify(const QString &imagePath);
    QString getPathForImage(const QString& digit);
};