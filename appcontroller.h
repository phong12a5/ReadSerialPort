#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <serialreader.h>

class AppModel;

class AppController : public QObject
{
    Q_OBJECT

public:
    explicit AppController(QObject *parent = nullptr);

    void setModel(AppModel* model);

    Q_INVOKABLE void startReadPort(QString portName);
    Q_INVOKABLE void stopReadPort(QString portName);

private slots:
    void onPortReady();
    void onCurrentPortChanged(QString,QString);
    void onBaudRateChanged(int,int);
    void onDataUpdated(QString portName, QByteArray);

private:
    AppModel* mAppModel;
};

#endif // APPCONTROLLER_H
