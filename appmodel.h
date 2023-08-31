#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>

#define MAX_SIZE_RECORD_LIST 1800000
#define RECORD_VIEW_LIST 200

#define DISPLAY_UPDATE_PERIOD 20000 //ms

class RecordsTabelModel;

class AppModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList portList READ portList WRITE setPortList NOTIFY portListChanged)
    Q_PROPERTY(QString currentPort READ currentPort WRITE setCurrentPort NOTIFY currentPortChanged)
    Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(QString serialData READ serialData NOTIFY serialDataChanged)
    Q_PROPERTY(QList<QObject*> recordList READ recordList NOTIFY recordListChanged FINAL)
    Q_PROPERTY(QObject* tableModel READ tableModel NOTIFY tableModelChanged)
    Q_PROPERTY(int dataSize READ dataSize WRITE setDataSize NOTIFY dataSizeChanged FINAL)
    Q_PROPERTY(int pointerIndex READ pointerIndex WRITE setPointerIndex NOTIFY pointerIndexChanged FINAL)
    Q_PROPERTY(int pointerIndexForTable READ pointerIndexForTable WRITE setPointerIndexForTable NOTIFY pointerIndexForTableChanged)

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

    int dataSize() const;
    void setDataSize(int);

    int pointerIndex() const;
    void setPointerIndex(int);

    int pointerIndexForTable() const;
    void setPointerIndexForTable(int);

    QObject* tableModel() const;
    QList<QObject*> recordList() const;
    QStringList& rawRecordList();

    Q_INVOKABLE void makeTableModel(int tableHeight, int rowHeight);
    Q_INVOKABLE void makeRecordsModel(int lstViewHeight, int dlgHeight);

signals:
    void portListChanged();
    void currentPortChanged(QString,QString);
    void baudRateChanged(int,int);
    void serialDataChanged();
    void recordListChanged();
    void tableModelChanged();
    void dataSizeChanged();
    void pointerIndexChanged();
    void pointerIndexForTableChanged();

private:
    QStringList mPortList;
    QString mCurrentPort;
    int mBaudRate;
    QString mSerialData;
    int mDataSize;
    int mPointerIndex;
    int mPointerIndexForTable;
    QList<QObject*> mRecordList;
    RecordsTabelModel* mTableModel;
    QStringList mRawRecordList;
};

#endif // APPMODEL_H
