#include "portreader.h"
#include <log.h>
#include <QSerialPortInfo>
#include<windows.h>
#include <timeapi.h>
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
    if (mSerial.isOpen())  mSerial.clear(QSerialPort::AllDirections);}

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

inline static void SleepInMs(unsigned int ms) {
    LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER t1, t2;           // ticks
    double elapsedTime = 0;

    // get ticks per second
    QueryPerformanceFrequency(&frequency);

    QueryPerformanceCounter(&t1);
    while(elapsedTime >= ms){
        QueryPerformanceCounter(&t2);
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
    }
}

void PortReader::onStarted()
{
    mRunningLock.lock();
    mIsRunning = true;
    mRunningLock.unlock();

    QElapsedTimer elapTime;

    while (true) {
        // start timer
        //        QueryPerformanceCounter(&t1);
        mRunningLock.lock();
        if (!mIsRunning) {
            mRunningLock.unlock();
            break;
        }
        mRunningLock.unlock();

        elapTime.start();
        onReadData();
        qint64 benchmark = elapTime.nsecsElapsed();
        LOGD(TAG) << "Done with benchmark:" << benchmark << "ns";

//        QThread::usleep(1000);
    }
}

bool PortReader::onReadData()
{
    if (!mSerial.isOpen()) {
        mSerial.open(QIODevice::ReadOnly);
        if (mSerial.isOpen())  mSerial.clear(QSerialPort::AllDirections);
        else return false;
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
        if (numRead > 0) {
            LOGD(TAG) << "numRead: " << numRead;
            emit sigDataReady(mPort, buffer);
            return true;
        }
    }
    return false;
}
