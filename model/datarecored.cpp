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
    if(mIndex < AppModel::instance()->rawRecordList().size()) {
        data = AppModel::instance()->rawRecordList().at(mIndex);
    }
    return data;
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
