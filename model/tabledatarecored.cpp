#include "tabledatarecored.h"
#include <appmodel.h>
#include <log.h>

static constexpr const char* const TAG = "AppController";

TableDataRecored::TableDataRecored(uint index)
    : QObject{nullptr},
    mIndex(index)
{

}

void TableDataRecored::reload()
{
    int targetIndex = mIndex + AppModel::instance()->pointerIndexForTable();
    if(targetIndex < AppModel::instance()->rawRecordList().size()) {
        mPackageData = AppModel::instance()->rawRecordList().at(targetIndex);
        format();
    }
}

bool TableDataRecored::format()
{
    mPacketCounter =  mPackageData.mid(14, 4).toUInt(nullptr, 16);
    mSampleTime =  mPackageData.mid(25,8).toUInt(nullptr, 16);
    mSensorType = mPackageData.mid(32, 4).toUInt(nullptr, 16);
    mGX= mPackageData.mid(38, 8).toFloat();
    mGY = mPackageData.mid(46, 8).toFloat();
    mGZ = mPackageData.mid(54, 8).toFloat();
    mAX = mPackageData.mid(38, 8).toFloat();
    mAY = mPackageData.mid(46, 8).toFloat();
    mAZ = mPackageData.mid(54, 8).toFloat();
    mCheckSum = mPackageData.mid(62, 2).toUInt(nullptr, 16);
    mSizeOfSensor = mPackageData.mid(36,2).toUInt(nullptr, 16);
    LOGD(TAG) << mPackageData;
    return true;
}
