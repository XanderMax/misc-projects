#pragma once

#include <QObject>
#include <iostream>

class MyGadget
{
    Q_GADGET
    int* p = nullptr;
public:
    MyGadget() {
        p = new int(1);
        std::cout << "()this=" << (void*)this << " p=" <<(void*)p << std::endl;
    }
    ~MyGadget() {
        std::cout << "~()this=" << (void*)this << " p=" <<(void*)p << std::endl;
        *p = 10;
        delete p;
    }

    MyGadget& operator=(const MyGadget& o)
    {
        *p = *o.p;
        std::cout << "(=)this=" << (void*)this << " p=" << (void*)p
                  << " o=" << (void*)&o << " o.p=" << (void*)o.p << std::endl;
        return *this;
    }
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