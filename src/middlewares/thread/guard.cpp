#include "middlewares/thread/guard.h"

thread::Guard::Guard(SemaphoreHandle_t mtx)
{
    this->mtx = mtx;
    xSemaphoreTake(this->mtx, portMAX_DELAY);
}

thread::Guard::~Guard()
{
    xSemaphoreGive(this->mtx);
}