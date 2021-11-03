#include <QGuiApplication>
#include <QObject>
#include <QPointer>
#include <QDebug>
#include <QTimer>
#include <functional>
#include <memory>

class MyObj : public QObject
{
    public:
    MyObj(const char* objName, QObject* parent = nullptr) : QObject(parent)
    {
        this->setObjectName(objName);
        qDebug() << "\tallocated MyObj -" << this->objectName() << "-" << (void*)this;
    }
    ~MyObj()
    {
        qDebug() << "\tdeleting MyObj - " << this->objectName() << "-" << (void*)this;
    }
};

class UseCases
{
    using Functor = std::function<void(QPointer<QObject>&)>;
    Functor f;

    std::function<void()> bind(const char* useCaseName, QObject* obj)
    {
        return [this, useCaseName, ptr = QPointer<QObject>(obj)]() mutable {
            qDebug() << useCaseName << ":";
            f(ptr);
        };
    }

public:
    template <class F> UseCases(F&& f) : f(std::forward<F>(f)) {}

    // this one in unrecommended because obj is allocated on stack and f calls delete on a pointer
    void useCase_1(int duration) 
    {
        qDebug() << "useCase_1" << ":";
        MyObj obj("useCase_1");
        QTimer::singleShot(duration, bind("useCase_1", &obj));
    }

    void useCase_2(int duration)
    {
        qDebug() << "useCase_2" << ":";
        QObject* obj = new MyObj("useCase_2");
        QTimer::singleShot(duration, bind("useCase_2", obj));
    }

    void useCase_3(int duration)
    {
        qDebug() << "useCase_3" << ":";
        std::unique_ptr<MyObj> ptr = std::make_unique<MyObj>("useCase_3");
        QTimer::singleShot(duration, bind("useCase_3", ptr.get()));
    }

    void useCase_4(int duration)
    {
        qDebug() << "useCase_4" << ":";
        QObject* obj = new MyObj("useCase_4");
        QTimer::singleShot(duration, bind("useCase_4", obj));
        obj->deleteLater();
    }

    void useCase_5(int duration)
    {
        qDebug() << "useCase_5" << ":";
        QTimer::singleShot(duration, [this](){
            QObject* obj = new MyObj("useCase_5");
            obj->deleteLater();
            bind("useCase_5", obj)();
        });
    }

    // this causes memory leak
    void useCase_6() 
    {
        qDebug() << "useCase_6" << ":";
        QPointer<QObject> ptr = new MyObj("useCase_6");
        Q_UNUSED(ptr);
    }

    void useCase_7()
    {
        qDebug() << "useCase_7" << ":";
        QPointer<QObject> ptr = new MyObj("useCase_7");
        ptr->deleteLater();
    }

    void useCase_8(int duration)
    {
        qDebug() << "useCase_8" << ":";
        QObject obj;
        QPointer<QObject> ptr = new MyObj("useCase_8", &obj);
        QTimer::singleShot(duration, bind("useCase_8", ptr.data()));
    }

    // this causes undefined behavior (most likely SIGSEGV)
    void useCase_9(int duration)
    {
        qDebug() << "useCase_9" << ":";
        QObject *obj;

        {
            MyObj mo("useCase_9");
            obj = &mo;
        }

        QPointer<QObject> ptr = obj;
        QTimer::singleShot(duration, bind("useCase_9", ptr.data()));
    }

    // this causes undefined behavior (most likely SIGSEGV)
    void useCase_10(int duration)
    {
        qDebug() << "useCase_10" << ":";
        QObject *obj = new MyObj("useCase_10");
        obj->deleteLater();

        QTimer::singleShot(duration, [this, duration, obj](){
            QTimer::singleShot(duration, bind("useCase_10", obj));
        });
    }

    // this causes undefined behavior (most likely SIGSEGV)
    void useCase_11(int duration)
    {
        qDebug() << "useCase_11" << ":";
        QObject *obj = new MyObj("useCase_11");
        delete obj;

        QTimer::singleShot(duration, bind("useCase_11", obj));
    }
};

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    auto lambda = [](QPointer<QObject>& ptr) {
        if (ptr) {
            qDebug() << "\tQPointer is not null - " << (void*)ptr;
            delete ptr;
        } else {
            qDebug() << "\tQPointer is null - " << (void*)ptr;
        }
    };

    UseCases uc {lambda};

    uc.useCase_1(1000);
    uc.useCase_2(1500);
    uc.useCase_3(2000);
    uc.useCase_4(2500);
    uc.useCase_5(2500);
    uc.useCase_6();
    uc.useCase_7();
    uc.useCase_8(3000);
    uc.useCase_9(3500);
    uc.useCase_10(4000);
    //uc.useCase_11(4500);

    app.exec();
}

/*
Possible output:
useCase_1 :
        allocated MyObj - "useCase_1" - 0x7ffc27578380
        deleting MyObj -  "useCase_1" - 0x7ffc27578380
useCase_2 :
        allocated MyObj - "useCase_2" - 0x559d50d89ac0
useCase_3 :
        allocated MyObj - "useCase_3" - 0x559d50e1ceb0
        deleting MyObj -  "useCase_3" - 0x559d50e1ceb0
useCase_4 :
        allocated MyObj - "useCase_4" - 0x559d50e1ceb0
useCase_5 :
useCase_6 :
        allocated MyObj - "useCase_6" - 0x559d50e965b0
useCase_7 :
        allocated MyObj - "useCase_7" - 0x559d50e67a70
useCase_8 :
        allocated MyObj - "useCase_8" - 0x559d50e1d4b0
        deleting MyObj -  "useCase_8" - 0x559d50e1d4b0
useCase_9 :
        allocated MyObj - "useCase_9" - 0x7ffc27578370
        deleting MyObj -  "useCase_9" - 0x7ffc27578370
useCase_10 :
        allocated MyObj - "useCase_10" - 0x559d50e167f0
        deleting MyObj -  "useCase_4" - 0x559d50e1ceb0
        deleting MyObj -  "useCase_7" - 0x559d50e67a70
        deleting MyObj -  "useCase_10" - 0x559d50e167f0
useCase_1 :
        QPointer is null -  0x0
useCase_2 :
        QPointer is not null -  0x559d50d89ac0
        deleting MyObj -  "useCase_2" - 0x559d50d89ac0
useCase_3 :
        QPointer is null -  0x0
useCase_4 :
        QPointer is null -  0x0
        allocated MyObj - "useCase_5" - 0x7f35f0005e40
useCase_5 :
        QPointer is not null -  0x7f35f0005e40
        deleting MyObj -  "useCase_5" - 0x7f35f0005e40
useCase_8 :
        QPointer is null -  0x0
useCase_9 :
        QPointer is not null -  0x7ffc27578370
Segmentation fault (core dumped)
*/