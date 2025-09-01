#include "middlewares/storage/guard.h"

storage::Guard::Guard(SemaphoreHandle_t mtx)
{
    this->mtx = mtx;
    xSemaphoreTake(this->mtx, portMAX_DELAY);
}

storage::Guard::~Guard()
{
    xSemaphoreGive(this->mtx);
}