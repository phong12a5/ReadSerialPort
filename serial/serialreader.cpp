#include "serialreader.h"
#include <QMutex>
#include <log.h>
#include <portreader.h>

static constexpr const char* const TAG = "SerialReader";

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

QStringList SerialReader::getListAvailPorts() const
{
    return mAvailablePorts;
}

void SerialReader::startReadPort(QString portName, int baudRate)
{
    LOGD(TAG) << "portName:" << portName << ", baudRate:" <<  baudRate;
    PortReader* reader = getExistedReader(portName);
    if (!reader) {
        reader = new PortReader(portName, baudRate);
        connect(reader, &PortReader::sigDataReady, this, &SerialReader::sigDataUpdated);
        mReaderList.insert(portName, reader);
    }
    if (reader->isRunning()) {
        reader->setBaudRate(baudRate);
    } else {
        reader->start();
    }
}

void SerialReader::stopReadPort(QString portName)
{
    LOGD(TAG) << "portName:" << portName;
    PortReader* reader = getExistedReader(portName);
    if (reader) {
        reader->stop();
    }
}

PortReader *SerialReader::getExistedReader(QString portName) const
{
    return mReaderList.value(portName, nullptr);
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
        emit sigPortAvailable();
    }
}
