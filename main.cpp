#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <appcontroller.h>
#include <appmodel.h>
#include <QQmlContext>
#include <log.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    AppController* appCtrl = new AppController();
    appCtrl->setModel(AppModel::instance());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("APP_CTRL", appCtrl);
    engine.rootContext()->setContextProperty("APP_MODEL", AppModel::instance());

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
