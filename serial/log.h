#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <QDebug>
#include <QThread>

#define LOGD(TAG) qDebug() << "[" << TAG << "] [" << QThread::currentThreadId() << "][" << __FUNCTION__ << "][" << __LINE__ << "] "

#endif // LOG_H
