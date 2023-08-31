#include "recordstabelmodel.h"
#include <tabledatarecored.h>
#include <log.h>

RecordsTabelModel::RecordsTabelModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    mItems.clear();
}

int RecordsTabelModel::rowCount(const QModelIndex &) const
{
    return mItems.size();
}

int RecordsTabelModel::columnCount(const QModelIndex &) const
{
    return E_COL_TOTAL;
}

QVariant RecordsTabelModel::data(const QModelIndex &index, int role) const
{
    QVariant data;
    {
        switch (role) {
        case Qt::DisplayRole: {
            TableDataRecored* record = static_cast<TableDataRecored*>(mItems.at(index.row()));
            switch (index.column()) {
            case E_COL_PACKET_DATA:
                data = record->packageData();
                break;
            case E_COL_PACKET_COUNTER:
                data = "\t" + QString::number(record->packetCounter()) + "\t";
                break;
            case E_COL_SENSOR_TYPE:
                data = "\t" + QString::number(record->sensorType()) + "\t";
                break;
            case E_COL_SIZE_OF_SENSOR:
                data = "\t" + QString::number(record->sizeOfSensor()) + "\t";
                break;
            case E_COL_AX:
                data = record->aX();
                break;
            case E_COL_AY:
                data = record->aY();
                break;
            case E_COL_AZ:
                data = record->aZ();
                break;
            case E_COL_GX:
                data = record->gX();
                break;
            case E_COL_GY:
                data = record->gY();
                break;
            case E_COL_GZ:
                data = record->gX();
                break;
            case E_COL_CHECK_SUM:
                data = "\t" + QString::number(record->checkSum()) + "\t";
                break;
            default:
                break;
            }
        }
        default:
            break;
        }

        return data;
    }
}

QVariant RecordsTabelModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (section) {
    case E_COL_PACKET_DATA:
        return "Packet data";
    case E_COL_PACKET_COUNTER:
        return "Packet counter";
    case E_COL_SENSOR_TYPE:
        return "Sensor type";
    case E_COL_SIZE_OF_SENSOR:
        return "Size sensor";
    case E_COL_AX:
        return "Ax";
    case E_COL_AY:
        return "Ay";
    case E_COL_AZ:
        return "Az";
    case E_COL_GX:
        return "Gx";
    case E_COL_GY:
        return "Gy";
    case E_COL_GZ:
        return "Gz";
    case E_COL_CHECK_SUM:
        return "Check sum";
    default:
        break;
    }
    return QVariant();
}

void RecordsTabelModel::makeModelItem(int tableHeight, int rowHeight)
{
    beginResetModel();
    int modelSize = tableHeight/rowHeight;
    if (modelSize > mItems.size()) {
        while(mItems.size() < modelSize) {
            mItems.append(new TableDataRecored(mItems.size()));
        }
    } else {
        while( mItems.size() && mItems.size() > modelSize) {
            QObject* record = mItems.takeLast();
            if (record) record->deleteLater();
        }
    }
    endResetModel();
}

void RecordsTabelModel::refreshModel()
{
    beginResetModel();
    for(int i = 0; i < mItems.size(); i++) {
        TableDataRecored* record = static_cast<TableDataRecored*>(mItems.at(i));
        record->reload();
    }
    endResetModel();
}
