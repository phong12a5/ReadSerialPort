#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

class SerialReader : public QObject
{
    Q_OBJECT
private:
    explicit SerialReader(QObject *parent = nullptr);

public:
    static SerialReader* instance();

    QStringList getListAvailPorts(bool* ok = nullptr) const;

signals:
    void portAvailable();

private slots:
    void onScanPort();

private:
    QTimer mScanPortTimer;
    QStringList mAvailablePorts;
};

#endif // SERIALREADER_H
