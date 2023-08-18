#include "portreader.h"
#include <log.h>

static constexpr const char* const TAG = "PortReader";

PortReader::PortReader(QObject *parent, int port, int baudRate)
    : QObject{parent},
      mPort(port),
      mBaudRate(baudRate),
      mThread(nullptr),
      mReadTimer(nullptr)
{
    LOGD(TAG) << "port:" << port << ", baudRate:" << baudRate;
    mSerial.setPortName(QString::number(port));
    mSerial.setBaudRate(baudRate);
}

PortReader::~PortReader()
{
    if (mThread) mThread->deleteLater();
}

void PortReader::start()
{
    LOGD(TAG);
    if (!mThread) {
        mThread = new QThread(this);
        connect(mThread, &QThread::started, this, &PortReader::onStarted);
        this->moveToThread(mThread);
        mThread->start();
    }
}

void PortReader::stop()
{
    LOGD(TAG);
    if (mThread) {
        mReadTimer->stop();
        mThread->quit();
        mThread->wait();
    }
}

void PortReader::onStarted()
{
    LOGD(TAG);
    if (!mReadTimer) {
        mReadTimer = new QTimer(this);
    }

    if (!mReadTimer->isActive()) {
        connect(mReadTimer, &QTimer::timeout, this, &PortReader::onReadData);
        mReadTimer->setSingleShot(false);
        mReadTimer->setInterval(READ_DATA_PERIOD);
        mReadTimer->start();
    }
}

void PortReader::onReadData()
{
    LOGD(TAG);
    QByteArray data;
    int numRead = 0, numReadTotal = 0;
    char buffer[50];

    if (!mSerial.isReadable()) {
        LOGD(TAG) << "port: " << mPort << "is not ready for reading";
        return;
    }

    for (;;) {
        numRead  = mSerial.read(buffer, 50);
        data.append(buffer);
        numReadTotal += numRead;
        if (numRead == 0 && !mSerial.waitForReadyRead()) {
            break;
        }
    }
    if (numReadTotal > 0){
        emit sigDataReady(data);
    }
}
