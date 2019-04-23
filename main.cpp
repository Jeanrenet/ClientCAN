#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "CBusCanInterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    CBusCanInterface *busCanInterface = new CBusCanInterface();

    QQmlApplicationEngine engine;

    //export des classes vers QML
    engine.rootContext()->setContextProperty("busCanInterface", busCanInterface);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
