#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    qDebug() << "Import paths:";
    for (const auto& path : engine.importPathList())
    {
        qDebug() << path;
    }
    engine.load(QUrl("qrc:/videoanalytics/qml/Main.qml"));

    return app.exec();
}
