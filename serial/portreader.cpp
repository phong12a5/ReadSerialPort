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
    mSerial.setDataBits(QSerialPort::Data8);
    mSerial.setParity(QSerialPort::NoParity);
    mSerial.setFlowControl(QSerialPort::HardwareControl);
    mSerial.setStopBits(QSerialPort::OneStop);
    mSerial.open(QIODevice::ReadOnly);
    if (mSerial.isOpen())  mSerial.clear(QSerialPort::Input);

    QObject::connect(&mSerial, &QSerialPort::readyRead, [&]
                     {
                         QByteArray datas = mSerial.readAll();
                         emit sigDataReady(mPort, datas);
                     });
}

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
    LOGD(TAG);
    /*
        mRunningLock.lock();
        mIsRunning = true;
        mRunningLock.unlock();

        static QElapsedTimer elapTime;

        while (true) {
            elapTime.restart();
            if (!mIsRunning) {
                mSerial.close();
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
    */
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

    if (!mSerial.isReadable()) {
        LOGD(TAG) << "cannot read device!";
        return false;
    }

    if (mSerial.baudRate() != mBaudRate) {
        LOGD(TAG) << "update baud date";
        LOGD(TAG) << "port: " << mPort << "is not ready for reading";
        return false;
    }

    //    int bufferSize = 100;
    int numRead = 0;

    LOGD(TAG) << "wait";
    if(mSerial.waitForReadyRead(1)) {
        int availNum = mSerial.bytesAvailable();
        char buffer[availNum];
        LOGD(TAG) << "read all";
        numRead =  mSerial.read(buffer, availNum);
        LOGD(TAG) << "buffer: " <<QByteArray(buffer).toHex(' ');
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
