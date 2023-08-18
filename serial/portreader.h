#ifndef PORTREADER_H
#define PORTREADER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QSerialPort>

#define READ_DATA_PERIOD 1000 //ms

class PortReader : public QObject
{
    Q_OBJECT
public:
    explicit PortReader(QObject *parent, int port, int baudRate);
    ~PortReader();

    void start();
    void stop();

signals:
    void sigDataReady(QByteArray);

public slots:
    void onStarted();
    void onReadData();

private:
    int mPort;
    int mBaudRate;
    QThread* mThread;
    QTimer* mReadTimer;
    QSerialPort mSerial;
};

#endif // PORTREADER_H
