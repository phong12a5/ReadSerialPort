#include "appio.h"
#include <log.h>
#include <QKeyEvent>
#include <QClipboard>
#include <appmodel.h>

static constexpr const char* const TAG = "AppIO";

AppIO::AppIO(QGuiApplication *app)
    : QObject{app},
      globalApp(app),
      mKeyboardModifiers(Qt::NoModifier)
{
    if(globalApp) {
        globalApp->installEventFilter(this);
    }
}

bool AppIO::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = (QWheelEvent*)event;
        AppModel* model = AppModel::instance();
        if(wheelEvent->angleDelta().y() > 0) { // up Wheel
            if (model->pointerIndex() > 0) model->setPointerIndex(model->pointerIndex() - 1);
        } else if(wheelEvent->angleDelta().y() < 0) { //down Wheel
            if (model->pointerIndex() < model->dataSize()) model->setPointerIndex(model->pointerIndex() + 1);
        }
    }
    return QObject::eventFilter(watched,event);
}

Qt::KeyboardModifiers AppIO::keyboardModifiers() const
{
    return mKeyboardModifiers;
}

void AppIO::sendToClibboard(QString data)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(data);
}

void AppIO::setKeyboardModifiers(Qt::KeyboardModifiers key)
{
    if(key != mKeyboardModifiers) {
        mKeyboardModifiers = key;
        emit keyboardModifiersChanged();
    }
}
