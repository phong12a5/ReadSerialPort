#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>

#define MAX_SIZE_RECORD_LIST 20000
#define DISPLAY_UPDATE_PERIOD 20000 //ms

class AppModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList portList READ portList WRITE setPortList NOTIFY portListChanged)
    Q_PROPERTY(QString currentPort READ currentPort WRITE setCurrentPort NOTIFY currentPortChanged)
    Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(QString serialData READ serialData NOTIFY serialDataChanged)
    Q_PROPERTY(QList<QObject*> recordList READ recordList NOTIFY recordListChanged FINAL)

private:
    explicit AppModel(QObject *parent = nullptr);

public:
    static AppModel *instance();

    QStringList portList() const;
    void setPortList(QStringList);

    QString currentPort() const;
    void setCurrentPort(QString);

    int baudRate() const;
    void setBaudRate(int);

    QString& serialData();
    void setSerialData(QString&);

    QList<QObject*> recordList() const;
    QStringList& rawRecordList();

signals:
    void portListChanged();
    void currentPortChanged(QString,QString);
    void baudRateChanged(int,int);
    void serialDataChanged();
    void recordListChanged();

private:
    QStringList mPortList;
    QString mCurrentPort;
    int mBaudRate;
    QString mSerialData;
    QList<QObject*> mRecordList;
    QStringList mRawRecordList;
};

#endif // APPMODEL_H
