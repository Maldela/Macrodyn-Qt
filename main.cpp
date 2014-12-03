#include <QApplication>
#include <QQmlApplicationEngine>
#include "Graphics/macrodyngraphicsitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<MacrodynGraphicsItem>("graphics", 1, 0, "MacrodynGraphicsItem");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
