#pragma once

#include <QObject>
#include <iostream>

class MyGadget
{
    Q_GADGET
public:
    MyGadget() {std::cout << "() " << (void*) this << std::endl;}
    ~MyGadget() {
        std::cout << "~() " << (void*) this << std::endl;
    }
    //MyGadget& operator=(MyGadget&&) {return *this;}
};

class MySingleton : public QObject
{
    Q_OBJECT
public:
    MySingleton(QObject* parent = nullptr) 
    : QObject(parent)
    {}

    Q_INVOKABLE MyGadget get() const {
        return {};
    }
};