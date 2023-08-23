#ifndef DATARECORED_H
#define DATARECORED_H

#include <QObject>

class DataRecored : public QObject
{
    Q_OBJECT
public:
    explicit DataRecored(uint index, QObject *parent = nullptr);

    Q_PROPERTY(QString raw READ raw NOTIFY rawChanged FINAL)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged FINAL)

    uint index() const;
    QString raw() const;

    bool visible() const;
    void setVisible(bool);

signals:
    void rawChanged();
    void visibleChanged();

private:
    uint mIndex;
    bool mVisible;

};

#endif // DATARECORED_H
