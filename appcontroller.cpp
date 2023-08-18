#include "appcontroller.h"
#include <appmodel.h>
#include <log.h>

static constexpr const char* const TAG = "AppController";

AppController::AppController(QObject *parent)
    : QObject{parent}
{
    connect(SerialReader::instance(), &SerialReader::portAvailable, this, &AppController::onPortReady);
}

void AppController::setModel(AppModel *model)
{
    mAppModel = model;
}

QStringList AppController::portList() const
{
    return SerialReader::instance()->getListAvailPorts();
}

void AppController::onPortReady()
{
    LOGD(TAG);
}
