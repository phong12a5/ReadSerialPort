#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>

class AppModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList portList READ portList WRITE setPortList NOTIFY portListChanged)
    Q_PROPERTY(QString currentPort READ currentPort WRITE setCurrentPort NOTIFY currentPortChanged)
    Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(QStringList serialData READ serialData NOTIFY serialDataChanged)

public:
    explicit AppModel(QObject *parent = nullptr);

    QStringList portList() const;
    void setPortList(QStringList);

    QString currentPort() const;
    void setCurrentPort(QString);

    int baudRate() const;
    void setBaudRate(int);

    QStringList serialData() const;
    void clearSerialData();
    void appendSerialData(QString);

signals:
    void portListChanged();
    void currentPortChanged(QString,QString);
    void baudRateChanged(int,int);
    void serialDataChanged();

private:
    QStringList mPortList;
    QString mCurrentPort;
    int mBaudRate;
    QStringList mSerialData;

};

#endif // APPMODEL_H
