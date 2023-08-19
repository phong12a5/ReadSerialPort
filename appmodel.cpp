#include "appmodel.h"
#include <QTimer>

AppModel::AppModel(QObject *parent)
    : QObject{parent}
{

}

QStringList AppModel::portList() const
{
    return mPortList;
}

void AppModel::setPortList(QStringList ports)
{
    if (ports != mPortList) {
        mPortList = ports;
        emit portListChanged();
    }
}

QString AppModel::currentPort() const
{
    return mCurrentPort;
}

void AppModel::setCurrentPort(QString data)
{
    if (data != mCurrentPort) {
        QString old = mCurrentPort;
        mCurrentPort = data;
        emit currentPortChanged(old, mCurrentPort);
    }
}

int AppModel::baudRate() const
{
    return mBaudRate;
}

void AppModel::setBaudRate(int baudRate)
{
    if (mBaudRate != baudRate) {
        int old = mBaudRate;
        mBaudRate = baudRate;
        emit baudRateChanged(old, mBaudRate);
    }
}

QStringList AppModel::serialData() const
{
    return mSerialData;
}

void AppModel::clearSerialData() {
    mSerialData.clear();
    emit serialDataChanged();
}

void AppModel::appendSerialData(QString data)
{
    if(mSerialData.size() > 100) mSerialData.takeFirst();
    mSerialData.append(data);
    static QTimer* timer = nullptr;
    if(timer == nullptr) {
        timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(100); // 50 fps
        connect(timer, &QTimer::timeout, this, [this] () { emit serialDataChanged(); });
    }
    if (!timer->isActive()) timer->start();
}
