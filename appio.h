#ifndef APPIO_H
#define APPIO_H

#include <QObject>
#include <QGuiApplication>

class AppIO : public QObject
{
    Q_OBJECT
public:
    explicit AppIO(QGuiApplication *app = nullptr);

    Q_PROPERTY(Qt::KeyboardModifiers keyboardModifiers READ keyboardModifiers NOTIFY keyboardModifiersChanged)

    bool eventFilter(QObject *watched, QEvent *event) override;

    Qt::KeyboardModifiers keyboardModifiers() const;

    Q_INVOKABLE void sendToClibboard(QString);

signals:
    void keyboardModifiersChanged();

    void keyboardSelectAll();
    void keyboardCopy();

private:
    void setKeyboardModifiers(Qt::KeyboardModifiers key);

private:
    QGuiApplication* globalApp;

    Qt::KeyboardModifiers mKeyboardModifiers;

};

#endif // APPIO_H
