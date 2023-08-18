#include "serialreader.h"
#include <QMutex>

static SerialReader* sInstance;

SerialReader::SerialReader(QObject *parent)
    : QObject{parent}
{
    mAvailablePorts.clear();
    onScanPort();

    mScanPortTimer.setSingleShot(false);
    mScanPortTimer.setInterval(2000);
    connect(&mScanPortTimer, &QTimer::timeout, this, &SerialReader::onScanPort);
    mScanPortTimer.start();
}

SerialReader *SerialReader::instance()
{
    if (!sInstance) {
        sInstance = new SerialReader(nullptr);
    }
    return sInstance;
}

QStringList SerialReader::getListAvailPorts(bool* ok) const
{
    return mAvailablePorts;
}

void SerialReader::onScanPort()
{
    bool changed;
    QStringList list;
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        list.append(port.portName());
    }

    if (mAvailablePorts.size() != list.size()) {
        changed = true;
    } else if (list != mAvailablePorts) {
        for(QString port : list) {
            if (!mAvailablePorts.contains(port)) {
                changed = true;
                break;
            }
        }
    }

    if (changed) {
        mAvailablePorts = list;
        emit portAvailable();
    }
}
