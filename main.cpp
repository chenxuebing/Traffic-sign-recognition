#include <QApplication>
#include <QQmlApplicationEngine>

#include "road_video_filter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<RoadVideoFilter>("roadVideoFilter", 1, 0, "RoadVideoFilter");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
