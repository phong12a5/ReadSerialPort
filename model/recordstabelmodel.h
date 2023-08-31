#ifndef RECORDSTABELMODEL_H
#define RECORDSTABELMODEL_H

#include <qqml.h>
#include <QAbstractTableModel>
#include <appmodel.h>

class TableDataRecored;
class RecordsTabelModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_ADDED_IN_MINOR_VERSION(1)

public:
    explicit RecordsTabelModel(QObject *parent = nullptr);

    enum E_TABLE_COLUMN_ID : int {
        E_COL_PACKET_DATA = 0,
        E_COL_PACKET_COUNTER,
        E_COL_SENSOR_TYPE,
        E_COL_SIZE_OF_SENSOR,
        E_COL_AX,
        E_COL_AY,
        E_COL_AZ,
        E_COL_GX,
        E_COL_GY,
        E_COL_GZ,
        E_COL_CHECK_SUM,
        E_COL_TOTAL
    };

    int rowCount(const QModelIndex & = QModelIndex()) const override;


    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override
    {
        return { {Qt::DisplayRole, "display"} };
    }

    void makeModelItem(int tableHeight, int rowHeight);
    void refreshModel();

private:
    QList<TableDataRecored*> mItems;
};

#endif // RECORDSTABELMODEL_H
