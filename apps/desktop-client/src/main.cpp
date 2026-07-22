#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "bootstrap/application_bootstrap.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    vap::ApplicationBootstrap bootstrap(engine);
    bootstrap.initialize();

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.load(QUrl("qrc:/VAP/qml/Main.qml"));

    return app.exec();
}
