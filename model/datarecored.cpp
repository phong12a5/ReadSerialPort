#include "datarecored.h"
#include <appmodel.h>
#include <log.h>

DataRecored::DataRecored(uint index, QObject *parent)
    : QObject{parent},
    mIndex(index),
    mVisible(false)
{

}

uint DataRecored::index() const
{
    return mIndex;
}

QString DataRecored::raw() const
{
    QString data = "";
    int targetIndex = mIndex + AppModel::instance()->pointerIndex();
    if(targetIndex < AppModel::instance()->rawRecordList().size()) {
        data = AppModel::instance()->rawRecordList().at(targetIndex);
    }
    return data.toUpper();
}

bool DataRecored::visible() const
{
    return mVisible;
}

void DataRecored::setVisible(bool dat)
{
    if (dat != mVisible) {
        mVisible = dat;
        emit visibleChanged();
    }
}
