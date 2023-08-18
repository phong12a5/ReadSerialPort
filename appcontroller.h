#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <serialreader.h>

class AppModel;

class AppController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList portList READ portList NOTIFY portListChanged)

public:
    explicit AppController(QObject *parent = nullptr);

    void setModel(AppModel* model);

    QStringList portList() const;
signals:
    void portListChanged();

private slots:
    void onPortReady();

private:
    AppModel* mAppModel;
};

#endif // APPCONTROLLER_H
