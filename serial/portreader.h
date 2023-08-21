#ifndef PORTREADER_H
#define PORTREADER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QSerialPort>
#include <QMutex>

#define READ_DATA_PERIOD 1000000 //unit: ns ~ 1 ms

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
    bool onReadData(bool* timeout = nullptr);

private:
    bool mIsRunning;
    QString mPort;
    int mBaudRate;
    QThread* mThread;
    QSerialPort mSerial;
    QMutex mRunningLock;
};

#endif // PORTREADER_H
