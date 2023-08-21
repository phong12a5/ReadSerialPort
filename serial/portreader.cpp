#include "portreader.h"
#include <log.h>
#include <QSerialPortInfo>
#include<windows.h>
#include <chrono>
#include <thread>
#include <QElapsedTimer>
#include <inttypes.h>


static constexpr const char* const TAG = "PortReader";

PortReader::PortReader(QString port, int baudRate)
    : mPort(port),
    mBaudRate(baudRate),
    mThread(nullptr)
{
    LOGD(TAG) << "port:" << port << ", baudRate:" << baudRate;
    mSerial.setPortName(port);
    mSerial.setBaudRate(baudRate);
    mSerial.setDataBits(QSerialPort::Data7);
    mSerial.setParity(QSerialPort::EvenParity);
    mSerial.setFlowControl(QSerialPort::HardwareControl);
    mSerial.setStopBits(QSerialPort::OneStop);
    mSerial.open(QIODevice::ReadOnly);
    if (mSerial.isOpen())  mSerial.clear(QSerialPort::Input);}

PortReader::~PortReader()
{
    if (mThread) mThread->deleteLater();
}

void PortReader::start()
{
    LOGD(TAG) << mPort;
    if (!mThread) {
        mThread = new QThread(this);
        connect(mThread, &QThread::started, this, &PortReader::onStarted);
        this->moveToThread(mThread);
    }

    if (!isRunning()) {
        mThread->start(QThread::TimeCriticalPriority);
    }
}

void PortReader::stop()
{
    LOGD(TAG) << mPort;
    if (mThread) {
        mRunningLock.lock();
        mIsRunning = false;
        mRunningLock.unlock();
        mThread->quit();
    }
}

bool PortReader::isRunning()
{
    return mThread && mThread->isRunning();
}

void PortReader::setBaudRate(int baudRate)
{
    mBaudRate = baudRate;
}

void PortReader::onStarted()
{
    mRunningLock.lock();
    mIsRunning = true;
    mRunningLock.unlock();

    static QElapsedTimer elapTime;

    while (true) {
        elapTime.restart();
        if (!mIsRunning) {
            break;
        }

        bool timeout = false;
        bool success = onReadData(&timeout);
        qint64 endTime = elapTime.nsecsElapsed();

        if (!timeout && endTime < READ_DATA_PERIOD) {
            while(elapTime.nsecsElapsed() < READ_DATA_PERIOD);
        }

        qint64 benchmark = elapTime.nsecsElapsed();

        LOGD(TAG) << (success? "Done" : "Failure") << " with benchmark:" << benchmark << "ns";
    }
}

bool PortReader::onReadData(bool* timeout)
{
    if (!mSerial.isOpen()) {
        mSerial.open(QIODevice::ReadOnly);
        if (mSerial.isOpen())  mSerial.clear(QSerialPort::Input);
        else {
            LOGD(TAG) << "cannot open device!";
            return false;
        }
    }

    if (mSerial.baudRate() != mBaudRate) {
        LOGD(TAG) << "update baud date";
        mSerial.setBaudRate(mBaudRate);
        mSerial.clear(QSerialPort::AllDirections);
    }

    if (!mSerial.isReadable()) {
        LOGD(TAG) << "port: " << mPort << "is not ready for reading";
        return false;
    }

    int bufferSize = 100;
    int numRead = 0;
    char buffer[bufferSize];

    if(mSerial.waitForReadyRead(1)) {
        numRead  = mSerial.read(buffer, bufferSize);
        bool succes = false;
        if (numRead > 0) {
            emit sigDataReady(mPort, buffer);
            succes = true;
        }
        return succes;
    } else {
        if (timeout) *timeout = true;
    }
    return false;
}
