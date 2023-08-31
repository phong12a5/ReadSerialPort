#ifndef TABLEDATARECORED_H
#define TABLEDATARECORED_H

#include <QObject>
#include <datarecored.h>

class TableDataRecored : public QObject
{
    Q_OBJECT
public:
    explicit TableDataRecored(uint index);

    Q_PROPERTY(QString packageData READ packageData NOTIFY packageDataChanged)
    Q_PROPERTY(int packetCounter READ packetCounter NOTIFY packetCounterChanged)
    Q_PROPERTY(int sampleTime READ sampleTime NOTIFY sampleTimeChanged)
    Q_PROPERTY(int sensorType READ sensorType NOTIFY sensorTypeChanged)
    Q_PROPERTY(int sizeOfSensor READ sizeOfSensor NOTIFY sizeOfSensorChanged)
    Q_PROPERTY(int checkSum READ checkSum NOTIFY checkSumChanged)
    Q_PROPERTY(float aX READ aX NOTIFY aXChanged)
    Q_PROPERTY(float aY READ aY NOTIFY aYChanged)
    Q_PROPERTY(float aZ READ aZ NOTIFY aZChanged)
    Q_PROPERTY(float gX READ gX NOTIFY gXChanged)
    Q_PROPERTY(float gY READ gY NOTIFY gYChanged)
    Q_PROPERTY(float gZ READ gZ NOTIFY gZChanged)

    QString packageData() const { return mPackageData; }
    int packetCounter() const { return mPacketCounter; }
    int sampleTime() const { return mSampleTime; }
    int sensorType() const { return mSensorType; }
    int sizeOfSensor() const { return mSizeOfSensor; }
    int checkSum() const { return mCheckSum; }
    float aX() const { return mAX; }
    float aY() const { return mAY; }
    float aZ() const { return mAZ; }
    float gX() const { return mGX; }
    float gY() const { return mGY; }
    float gZ() const { return mGZ; }

    void reload();

private:
    bool format();

signals:
    void packageDataChanged();
    void packetCounterChanged();
    void sampleTimeChanged();
    void sensorTypeChanged();
    void sizeOfSensorChanged();
    void checkSumChanged();
    void aXChanged();
    void aYChanged();
    void aZChanged();
    void gXChanged();
    void gYChanged();
    void gZChanged();

private:
    uint mIndex;
    DataRecored* mRawRecord;
    QString mPackageData;
    int mPacketCounter;
    int mSampleTime;
    int mSensorType;
    int mSizeOfSensor;
    int mCheckSum;
    float mAX;
    float mAY;
    float mAZ;
    float mGX;
    float mGY;
    float mGZ;
};

#endif // TABLEDATARECORED_H
