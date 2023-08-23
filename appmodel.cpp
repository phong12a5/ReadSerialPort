#include "appmodel.h"
#include <QTimer>
#include <log.h>
#include <model/datarecored.h>

static AppModel* sInstance = nullptr;

AppModel::AppModel(QObject *parent)
    : QObject{parent}
{
    mRawRecordList.clear();

    for (int i = 0; i < MAX_SIZE_RECORD_LIST; i++) {
        mRecordList.append(new DataRecored(i, this));
    }
}

AppModel *AppModel::instance() {
    if (!sInstance) {
        sInstance = new AppModel();
    }
    return sInstance;
}


QStringList AppModel::portList() const
{
    return mPortList;
}

void AppModel::setPortList(QStringList ports)
{
    if (ports != mPortList) {
        mPortList = ports;
        emit portListChanged();
    }
}

QString AppModel::currentPort() const
{
    return mCurrentPort;
}

void AppModel::setCurrentPort(QString data)
{
    if (data != mCurrentPort) {
        QString old = mCurrentPort;
        mCurrentPort = data;
        emit currentPortChanged(old, mCurrentPort);
    }
}

int AppModel::baudRate() const
{
    return mBaudRate;
}

void AppModel::setBaudRate(int baudRate)
{
    if (mBaudRate != baudRate) {
        int old = mBaudRate;
        mBaudRate = baudRate;
        emit baudRateChanged(old, mBaudRate);
    }
}

QString& AppModel::serialData()
{
    return mSerialData;
}

void AppModel::setSerialData(QString& data)
{
    if (mRawRecordList.size() >= MAX_SIZE_RECORD_LIST) {
        mRawRecordList.removeFirst();
    }

    mRawRecordList.append(data);


    static QTimer* timer = nullptr;
    if(timer == nullptr) {
        timer = new QTimer(this);
        timer->setSingleShot(false);
        timer->setInterval(1000); // 6 fps
        connect(timer, &QTimer::timeout, this, [this] () {
            emit recordListChanged();
            timer->start();

            if (timer->interval() == 1000) timer->setInterval(DISPLAY_UPDATE_PERIOD);

            for(int i = 0; i < mRecordList.size(); i++) {
                DataRecored* record = static_cast<DataRecored*>(mRecordList.at(i));
                if (record->visible()) {
                    emit record->rawChanged();
                }
            }
        });
        timer->start();
    }
}

QList<QObject*> AppModel::recordList() const
{
    return mRecordList;
}

QStringList &AppModel::rawRecordList()
{
    return mRawRecordList;
}
