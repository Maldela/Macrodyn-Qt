#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Graphics/macrodyngraphicsitem.h"
#include "logger.h"
#include "simloader.h"
//#include "documenthandler.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qRegisterMetaType<QSharedPointer<QImage> >();
    qRegisterMetaType<xyRange>();
    qmlRegisterType<MacrodynGraphicsItem>("MacrodynQML", 1, 0, "MacrodynGraphicsItem");
    //qmlRegisterType<DocumentHandler>("MacrodynQML", 1, 0, "DocumentHandler");
    //qmlRegisterType<SimEditor>("MacrodynQML", 1, 0, "SimEditor");
    qmlRegisterType<Logger>("MacrodynQML", 1, 0, "Logger");
    QQmlApplicationEngine engine;
    SimLoader loader;
    engine.rootContext()->setContextProperty("loader", &loader);
    engine.load(QUrl(QStringLiteral("qrc:/QML/main.qml")));
    return app.exec();
}
