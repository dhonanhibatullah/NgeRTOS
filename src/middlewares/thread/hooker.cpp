#include "middlewares/thread/hooker.h"

thread::Hooker::Hooker() : handler(NULL), mtx(NULL), is_set(false)
{
}

thread::Hooker::~Hooker()
{
    if (this->is_set)
        vSemaphoreDelete(this->mtx);
}

bool thread::Hooker::link(TaskHandle_t task_handle)
{
    if (task_handle == NULL || this->is_set)
        return false;

    this->handler = task_handle;
    this->is_set = true;
    this->mtx = xSemaphoreCreateMutex();
    return true;
}

bool thread::Hooker::release()
{
    if (!this->is_set)
        return false;

    thread::Guard ts(this->mtx);

    this->handler = NULL;
    this->is_set = false;
    return true;
}

bool thread::Hooker::hook(uint32_t code)
{
    if (!this->is_set)
        return false;

    thread::Guard ts(this->mtx);

    if (!this->handler)
        return false;

    xTaskNotify(this->handler, code, eSetValueWithOverwrite);
    return true;
}