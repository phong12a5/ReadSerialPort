#include "log.h"

static QElapsedTimer* elapTime = nullptr;

QElapsedTimer* getElapTime()  {
    if (!elapTime) {
        elapTime = new QElapsedTimer();
        elapTime->start();
    }
    return elapTime;
}
