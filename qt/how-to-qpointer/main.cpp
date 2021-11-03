#include <QGuiApplication>
#include <QObject>
#include <QPointer>
#include <QDebug>
#include <QTimer>
#include <functional>
#include <memory>
#include <QQmlApplicationEngine>
#include <QByteArray>
#include <QQmlComponent>
#include <QUrl>
#include <QQuickItem>

static const char* LINK = "https://doc.qt.io/qt-5/qpointer.html";

const char* QML_DATA = 
"import QtQuick 2.12\n"
"import QtQuick.Window 2.11\n"
"Window {\n"
"   id: root\n"
"   width: 1000\n"
"   height: 800\n"
"   visible: true \n"
"   Loader {\n"
"       id: _loader\n"
"       width: root.width\n"
"       height: root.height\n"
"       sourceComponent: _component\n"
"   }\n"
"   Component {\n"
"       id: _component\n"
"       Rectangle {width: 100; height: 100; color: \"#9ec3ff\"; objectName: \"rectangle\";}\n"
"   }\n"
"   Timer {\n"
"       interval: 6000\n"
"       running: true\n"
"       onTriggered: {\n"
"           console.log(\"deactivating _loader\")\n"
"           _loader.active = false\n"
"       }\n"
"   }\n"
"}\n"
;

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

class UseCaseLogger
{
    const char* useCaseName;
public:
    UseCaseLogger(const char* useCaseName) : useCaseName(useCaseName)
    {
        qDebug() << useCaseName << "beg";
    }
    ~UseCaseLogger()
    {
        qDebug() << useCaseName << "end";
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
        UseCaseLogger ucl{"useCase_1"};
        MyObj obj("useCase_1");
        QTimer::singleShot(duration, bind("useCase_1", &obj));
    }

    void useCase_2(int duration)
    {
        UseCaseLogger ucl{"useCase_2"};
        QObject* obj = new MyObj("useCase_2");
        QTimer::singleShot(duration, bind("useCase_2", obj));
    }

    void useCase_3(int duration)
    {
        UseCaseLogger ucl{"useCase_3"};
        std::unique_ptr<MyObj> ptr = std::make_unique<MyObj>("useCase_3");
        QTimer::singleShot(duration, bind("useCase_3", ptr.get()));
    }

    void useCase_4(int duration)
    {
        UseCaseLogger ucl{"useCase_4"};
        QObject* obj = new MyObj("useCase_4");
        QTimer::singleShot(duration, bind("useCase_4", obj));
        obj->deleteLater();
    }

    void useCase_5(int duration)
    {
        UseCaseLogger ucl{"useCase_5"};
        QTimer::singleShot(duration, [this](){
            QObject* obj = new MyObj("useCase_5");
            obj->deleteLater();
            bind("useCase_5", obj)();
        });
    }

    // this causes memory leak
    void useCase_6() 
    {
        UseCaseLogger ucl{"useCase_6"};
        QPointer<QObject> ptr = new MyObj("useCase_6");
        Q_UNUSED(ptr);
    }

    void useCase_7()
    {
        UseCaseLogger ucl{"useCase_7"};
        QPointer<QObject> ptr = new MyObj("useCase_7");
        ptr->deleteLater();
    }

    void useCase_8(int duration)
    {
        UseCaseLogger ucl{"useCase_8"};
        QObject obj;
        QPointer<QObject> ptr = new MyObj("useCase_8", &obj);
        QTimer::singleShot(duration, bind("useCase_8", ptr.data()));
    }

    // this causes undefined behavior (most likely SIGSEGV)
    void useCase_9(int duration)
    {
        UseCaseLogger ucl{"useCase_9"};
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
        UseCaseLogger ucl{"useCase_10"};
        QObject *obj = new MyObj("useCase_10");
        obj->deleteLater();

        QTimer::singleShot(duration, [this, duration, obj](){
            QTimer::singleShot(duration, bind("useCase_10", obj));
        });
    }

    // this causes undefined behavior (most likely SIGSEGV)
    void useCase_11(int duration)
    {
        UseCaseLogger ucl{"useCase_11"};
        QObject *obj = new MyObj("useCase_11");
        delete obj;

        QTimer::singleShot(duration, bind("useCase_11", obj));
    }

    void useCase_12(int duration) 
    {
        UseCaseLogger ucl{"useCase_12"};
        QObject *obj = new MyObj("useCase_12.parent");
        QObject *ch = new MyObj("useCase_12.child", obj);

        // note that parent will be deleted after child is deleted
        QTimer::singleShot(duration + 500, bind("useCase_12.parent", obj));
        QTimer::singleShot(duration, bind("useCase_12.child", ch));
    }
};

int main(int argc, char** argv)
{
    qDebug() << "Visit" << LINK << "for details";
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

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
    uc.useCase_9(8000);
    uc.useCase_10(9000);
    //uc.useCase_11(9500); // commented because otherwise crash happens too soon. check for yourself
    uc.useCase_12(5000);

    QQmlComponent component(&engine, &app);
    component.setData(QByteArray{QML_DATA, -1}, QUrl{});
    QObject* root = component.create();
    QQuickItem* rectangle = root->findChild<QQuickItem*>("rectangle");

    QTimer visibleTimer;
    visibleTimer.setInterval(500);

    //BAD: rectanlge is owned by QML and can be deleted at any point. will cause undefined behavior (most likely SIGSEGV)
    // QObject::connect(&visibleTimer, &QTimer::timeout, &visibleTimer, [=]() {
    //     if (rectangle) {
    //         rectangle->setVisible(!rectangle->isVisible());
    //     }
    // });

    //OK: QPointer will become nullptr as soon as QML item is deleted
    QObject::connect(&visibleTimer, &QTimer::timeout, &visibleTimer, [&, recPtr = QPointer<QQuickItem>(rectangle)]() {
        if (recPtr) {
            recPtr->setVisible(!recPtr->isVisible());
        } else {
            qDebug() << "visibleTimer: Rectanlge is null";
            visibleTimer.stop();
        }
    });

    visibleTimer.start();

    app.exec();
}

/*
Possible output:
Visit https://doc.qt.io/qt-5/qpointer.html for details
useCase_1 beg
        allocated MyObj - "useCase_1" - 0x7ffc75507b40
        deleting MyObj -  "useCase_1" - 0x7ffc75507b40
useCase_1 end
useCase_2 beg
        allocated MyObj - "useCase_2" - 0x55590f99bf10
useCase_2 end
useCase_3 beg
        allocated MyObj - "useCase_3" - 0x55590f8db590
        deleting MyObj -  "useCase_3" - 0x55590f8db590
useCase_3 end
useCase_4 beg
        allocated MyObj - "useCase_4" - 0x55590f8db590
useCase_4 end
useCase_5 beg
useCase_5 end
useCase_6 beg
        allocated MyObj - "useCase_6" - 0x55590f8bd340
useCase_6 end
useCase_7 beg
        allocated MyObj - "useCase_7" - 0x55590f8d1cd0
useCase_7 end
useCase_8 beg
        allocated MyObj - "useCase_8" - 0x55590f8d1cf0
        deleting MyObj -  "useCase_8" - 0x55590f8d1cf0
useCase_8 end
useCase_9 beg
        allocated MyObj - "useCase_9" - 0x7ffc75507b30
        deleting MyObj -  "useCase_9" - 0x7ffc75507b30
useCase_9 end
useCase_10 beg
        allocated MyObj - "useCase_10" - 0x55590f8d8e30
useCase_10 end
useCase_12 beg
        allocated MyObj - "useCase_12.parent" - 0x55590f920d50
        allocated MyObj - "useCase_12.child" - 0x55590f8d8cd0
useCase_12 end
        deleting MyObj -  "useCase_4" - 0x55590f8db590
        deleting MyObj -  "useCase_7" - 0x55590f8d1cd0
        deleting MyObj -  "useCase_10" - 0x55590f8d8e30
useCase_1 :
        QPointer is null -  0x0
useCase_2 :
        QPointer is not null -  0x55590f99bf10
        deleting MyObj -  "useCase_2" - 0x55590f99bf10
useCase_3 :
        QPointer is null -  0x0
useCase_4 :
        QPointer is null -  0x0
        allocated MyObj - "useCase_5" - 0x55590fc15150
useCase_5 :
        QPointer is not null -  0x55590fc15150
        deleting MyObj -  "useCase_5" - 0x55590fc15150
useCase_8 :
        QPointer is null -  0x0
useCase_12.child :
        QPointer is not null -  0x55590f8d8cd0
        deleting MyObj -  "useCase_12.child" - 0x55590f8d8cd0
useCase_12.parent :
        QPointer is not null -  0x55590f920d50
        deleting MyObj -  "useCase_12.parent" - 0x55590f920d50
qml: deactivating _loader
visibleTimer: Rectanlge is null
useCase_9 :
        QPointer is not null -  0x7ffc75507b30
Segmentation fault (core dumped)
*/