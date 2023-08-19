#ifndef LOG_H
#define LOG_H

#include <QDebug>
#include <QThread>
#include <QElapsedTimer>
#include <ostream>

#define LOGD(TAG) qDebug() << "[" << TAG << "][" << QThread::currentThreadId() << "][" << __FUNCTION__ << "][" << __LINE__ << "] "

#endif // LOG_H
