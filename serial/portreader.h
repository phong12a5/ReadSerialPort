#ifndef PORTREADER_H
#define PORTREADER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QSerialPort>

#define READ_DATA_PERIOD 1 //ms

class PortReader : public QObject
{
    Q_OBJECT
public:
    explicit PortReader(QString port, int baudRate);
    ~PortReader();

    void start();
    void stop();
    bool isRunning();
    void setBaudRate(int);

signals:
    void sigDataReady(QString portName, QByteArray);

public slots:
    void onStarted();
    void onReadData();

private:
    QString mPort;
    int mBaudRate;
    QThread* mThread;
    QTimer* mReadTimer;
    QSerialPort mSerial;
};

#endif // PORTREADER_H
