#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "traffic_sign_recognition.h"

int main(int argc, char *argv[])
{
//    test();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
