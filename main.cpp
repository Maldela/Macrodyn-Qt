#include <QApplication>
#include <QQmlApplicationEngine>
#include "Graphics/macrodyngraphicsitem.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<MacrodynGraphicsItem>("MacrodynQML", 1, 0, "MacrodynGraphicsItem");
    qmlRegisterType<Logger>("MacrodynQML", 1, 0, "Logger");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
