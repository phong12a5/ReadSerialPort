#include "appmodel.h"
#include <QTimer>
#include <log.h>
#include <model/datarecored.h>
#include <model/tabledatarecored.h>
#include <model/recordstabelmodel.h>

static constexpr const char* const TAG = "AppController";

static AppModel* sInstance = nullptr;

AppModel::AppModel(QObject *parent)
    : QObject{parent},
    mDataSize(0),
    mPointerIndex(0),
    mPointerIndexForTable(0)
{
    mRawRecordList.clear();
    mTableModel = new RecordsTabelModel(this);
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

    // split packets
    const QString partern = "FAFF";
    bool startByNewPacket = data.startsWith(partern);
    QStringList packets = data.split(partern);

    for(int i = 0; i < packets.size(); i++) {
        if(packets[i].isEmpty()) continue;

        if(i == 0 && !startByNewPacket) {
            if(mRawRecordList.empty()) {
                mRawRecordList.append(packets[i]);
            } else {
                // concat to last itemt
                QString lastRecord = mRawRecordList.last();
                mRawRecordList[mRawRecordList.size() - 1] = lastRecord + packets[i];
            }
        } else {
            mRawRecordList.append(partern +  packets[i]);
        }
    }


    static QTimer* timer = nullptr;
    if(timer == nullptr) {
        timer = new QTimer(this);
        timer->setSingleShot(false);
        timer->setInterval(1000);
        connect(timer, &QTimer::timeout, this, [this] () {

            emit recordListChanged();
            timer->start();

            if (timer->interval() == 1000) timer->setInterval(DISPLAY_UPDATE_PERIOD); // 6 fps

            for(int i = 0; i < mRecordList.size(); i++) {
                DataRecored* record = static_cast<DataRecored*>(mRecordList.at(i));
                if (record->visible()) {
                    emit record->rawChanged();
                }
            }

            mTableModel->refreshModel();

            setDataSize(mRawRecordList.size());
        });
        timer->start();
    }
}

int AppModel::dataSize() const
{
    return mDataSize;
}

void AppModel::setDataSize(int size)
{
    if (size != mDataSize) {
        mDataSize = size;
        emit dataSizeChanged();
    }
}

int AppModel::pointerIndex() const
{
    return mPointerIndex;
}

void AppModel::setPointerIndex(int idx)
{
    if (idx != mPointerIndex) {
        mPointerIndex = idx;
        emit pointerIndexChanged();

        for(int i = 0; i < mRecordList.size(); i++) {
            DataRecored* record = static_cast<DataRecored*>(mRecordList.at(i));
            if (record->visible()) {
                emit record->rawChanged();
            }
        }
    }
}

int AppModel::pointerIndexForTable() const
{
    return mPointerIndexForTable;
}

void AppModel::setPointerIndexForTable(int data)
{
    if (data != mPointerIndexForTable) {
        mPointerIndexForTable = data;
        emit pointerIndexChanged();

        mTableModel->refreshModel();
    }
}

QObject * AppModel::tableModel() const
{
    return (QObject *)mTableModel;
}

QList<QObject*> AppModel::recordList() const
{
    return mRecordList;
}

QStringList &AppModel::rawRecordList()
{
    return mRawRecordList;
}

void AppModel::makeTableModel(int tableHeight, int rowHeight)
{
    mTableModel->makeModelItem(tableHeight, rowHeight);
}


void AppModel::makeRecordsModel(int lstViewHeight, int dlgHeight)
{
    int modelSize = lstViewHeight/dlgHeight;
    if (modelSize > mRecordList.size()) {
        while(mRecordList.size() < modelSize) {
            mRecordList.append(new DataRecored(mRecordList.size(), this));
        }
        emit recordListChanged();
    } else {
        while( mRecordList.size() && mRecordList.size() > modelSize) {
            QObject* record = mRecordList.takeLast();
            if (record) record->deleteLater();
        }
        emit recordListChanged();
    }
}
