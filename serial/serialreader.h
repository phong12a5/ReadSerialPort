#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QMap>

class PortReader;
class SerialReader : public QObject
{
    Q_OBJECT
private:
    explicit SerialReader(QObject *parent = nullptr);

public:
    static SerialReader* instance();

    QStringList getListAvailPorts() const;
    void startReadPort(QString portName, int baudRate);
    void stopReadPort(QString portName);


private:
    PortReader* getExistedReader(QString portName) const;

signals:
    void sigPortAvailable();
    void sigDataUpdated(QString portName, QByteArray);

private slots:
    void onScanPort();

private:
    QTimer mScanPortTimer;
    QStringList mAvailablePorts;
    QMap<QString, PortReader*> mReaderList;
};

#endif // SERIALREADER_H
