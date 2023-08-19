#include "portreader.h"
#include <log.h>
#include <QSerialPortInfo>

#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions      */
#include <errno.h>   /* ERROR Number Definitions           */

static constexpr const char* const TAG = "PortReader";

PortReader::PortReader(QString port, int baudRate)
    : mPort(port),
      mBaudRate(baudRate),
      mThread(nullptr),
      mReadTimer(nullptr)
{
    LOGD(TAG) << "port:" << port << ", baudRate:" << baudRate;
//    mSerial.setPortName(port);
//    mSerial.setBaudRate(baudRate);
//    mSerial.setDataBits(QSerialPort::Data7);
//    mSerial.setParity(QSerialPort::EvenParity);
//    mSerial.setFlowControl(QSerialPort::HardwareControl);
//    mSerial.setStopBits(QSerialPort::OneStop);


//    mSerial.open(QIODevice::ReadOnly);
//    if (mSerial.isOpen())  mSerial.clear(QSerialPort::Input);

//    QObject::connect(&mSerial, &QSerialPort::readyRead, [&]
//                     {
//                         //this is called when readyRead() is emitted
//                         LOGD(TAG) << "New data available: " << mSerial.bytesAvailable();
////                         QByteArray datas = mSerial.readAll();
////                         mSerial.clear(QSerialPort::Input);
////                         LOGD(TAG) << QString(datas);
//                     });
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
    }

    if (!isRunning()) {
        mThread->start(QThread::TimeCriticalPriority);
    }
}

void PortReader::stop()
{
    LOGD(TAG);
    if (mThread) {
        if (mReadTimer && mReadTimer->isActive()) {
            mReadTimer->setSingleShot(true);
        }
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
    if (!mReadTimer) {
        mReadTimer = new QTimer(this);
    }

    if (!mReadTimer->isActive()) {
        connect(mReadTimer, &QTimer::timeout, this, &PortReader::onReadData);
        mReadTimer->setInterval(READ_DATA_PERIOD);
    }
    mReadTimer->setSingleShot(false);
    mReadTimer->start();
}

void PortReader::onReadData()
{
    LOGD(TAG);
//    if (!mSerial.isOpen()) {
//        mSerial.open(QIODevice::ReadOnly);
//        if (mSerial.isOpen())  mSerial.clear(QSerialPort::AllDirections);
//        else return;

//    }

//    if (mSerial.baudRate() != mBaudRate) {
//        LOGD(TAG) << "update baud date";
//        mSerial.setBaudRate(mBaudRate);
//        mSerial.clear(QSerialPort::AllDirections);
//    }

//    if (!mSerial.isReadable()) {
//        LOGD(TAG) << "port: " << mPort << "is not ready for reading";
//        return;
//    }


//    while(true) {
//        mSerial.waitForReadyRead();
//        qint64 bytesAvailable = mSerial.bytesAvailable();
//        if (bytesAvailable > 0) {
//            char buffer[2];
//            mSerial.read(buffer, 1);
//        }
//    }

//    if (mSerial.waitForReadyRead()) {
//        QByteArray datas = mSerial.readAll();
//        LOGD(TAG) << QString(datas);
//    }

//    QByteArray datas = mSerial.readAll();
//    if (!datas.isEmpty()) {
//        LOGD(TAG) << QString(datas);
////        emit sigDataReady(mPort, datas);
//    }

    int fd;           //device file id
    //------------------------------- Opening the Serial Port -------------------------------
    const char * portNameChr = mPort.toStdString().data();
    LOGD(TAG) << "mPort: " << mPort;
    LOGD(TAG) << "portNameChr: " << portNameChr;
    printf("portNameChr: %s",portNameChr);
    fd = open(portNameChr,O_RDWR | O_NOCTTY);    // ttyUSB0 is the FT232 based USB2SERIAL Converter
    if(fd == -1)                        // Error Checking
        printf("Error while opening the device\n");
    //---------- Setting the Attributes of the serial port using termios structure ---------
    struct termios SerialPortSettings;  // Create the structure
    tcgetattr(fd, &SerialPortSettings); // Get the current attributes of the Serial port
    // Setting the Baud rate
    cfsetispeed(&SerialPortSettings,mBaudRate); // Set Read  Speed as 19200
    cfsetospeed(&SerialPortSettings,mBaudRate); // Set Write Speed as 19200

    SerialPortSettings.c_cflag &= ~PARENB;   // Disables the Parity Enable bit(PARENB),So No Parity
    SerialPortSettings.c_cflag &= ~CSTOPB;   // CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit
    SerialPortSettings.c_cflag &= ~CSIZE;    // Clears the mask for setting the data size
    SerialPortSettings.c_cflag |=  CS8;      // Set the data bits = 8
    SerialPortSettings.c_cflag &= ~CRTSCTS;       // No Hardware flow Control
    SerialPortSettings.c_cflag |= CREAD | CLOCAL; // Enable receiver,Ignore Modem Control lines
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);  // Disable XON/XOFF flow control both i/p and o/p
    SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Non Cannonical mode
    SerialPortSettings.c_oflag &= ~OPOST;//No Output Processing
    // Setting Time outs
    SerialPortSettings.c_cc[VMIN] = 10; // Read at least 10 characters
    SerialPortSettings.c_cc[VTIME] = 0; // Wait indefinetly

    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) // Set the attributes to the termios structure
        printf("Error while setting attributes \n");
    //------------------------------- Read data from serial port -----------------------------

    char read_buffer[32];   // Buffer to store the data received
    int  bytes_read = 0;    // Number of bytes read by the read() system call
    int bytes_written = 0;  // Number of bytes written
    int i = 0;

//    tcflush(fd, TCIFLUSH);   // Discards old data in the rx buffer
    //Device intialization

//    char write_buffer[]="READ? \n ";
//    bytes_written=write(fd,&write_buffer,sizeof(write_buffer));


    bytes_read = read(fd,&read_buffer,32); // Read the data
    LOGD(TAG) << "bytes_read:" << bytes_read;

    for(i=0;i<10;i++)    //printing only the needed characters
        LOGD(TAG) << read_buffer[i];
    close(fd); // Close the serial port
}
