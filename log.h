#ifndef LOG_H
#define LOG_H

#include <QDebug>
#include <QThread>
#include <QElapsedTimer>



QElapsedTimer* getElapTime();

#define LOGD(TAG) qDebug() << "[" << getElapTime()->nsecsElapsed() << "][" << TAG << "][" << QThread::currentThreadId() << "][" << __FUNCTION__ << "][" << __LINE__ << "] "

#endif // LOG_H
