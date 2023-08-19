#include "appcontroller.h"
#include <appmodel.h>
#include <log.h>

static constexpr const char* const TAG = "AppController";

AppController::AppController(QObject *parent)
    : QObject{parent}
{
    LOGD(TAG);
    connect(SerialReader::instance(), &SerialReader::sigPortAvailable, this, &AppController::onPortReady);
    connect(SerialReader::instance(), &SerialReader::sigDataUpdated, this, &AppController::onDataUpdated);
}

void AppController::setModel(AppModel *model)
{
    if (mAppModel != model) {
        mAppModel = model;
        connect(mAppModel, &AppModel::currentPortChanged, this, &AppController::onCurrentPortChanged);
        connect(mAppModel, &AppModel::baudRateChanged, this, &AppController::onBaudRateChanged);
        mAppModel->setPortList(SerialReader::instance()->getListAvailPorts());
    }
}

void AppController::startReadPort(QString portName)
{
    SerialReader::instance()->startReadPort(portName, mAppModel->baudRate());
}

void AppController::stopReadPort(QString portName)
{
    SerialReader::instance()->stopReadPort(portName);
}

void AppController::onPortReady()
{
    LOGD(TAG);
    mAppModel->setPortList(SerialReader::instance()->getListAvailPorts());
}

void AppController::onCurrentPortChanged(QString oldPort, QString newPort)
{
    LOGD(TAG) << "oldPort:" << oldPort << ", newPort:" << newPort;
    mAppModel->clearSerialData();
    SerialReader::instance()->stopReadPort(oldPort);
    SerialReader::instance()->startReadPort(mAppModel->currentPort(), mAppModel->baudRate());
}

void AppController::onBaudRateChanged(int oldBaudRate, int newBaudRate)
{
    LOGD(TAG) << "oldBaudRate:" << oldBaudRate << ", newBaudRate:" << newBaudRate;
    SerialReader::instance()->startReadPort(mAppModel->currentPort(), mAppModel->baudRate());
}

void AppController::onDataUpdated(QString portName, QByteArray data)
{
    LOGD(TAG) << portName;
    if (portName != mAppModel->currentPort()) return;
    mAppModel->appendSerialData(data);
}
