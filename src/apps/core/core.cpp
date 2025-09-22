#include "apps/core/core.h"

core::Thread::Thread() : thread::Thread(
                             APPS_CORE_NAME,
                             APPS_CORE_STACK_SIZE,
                             APPS_CORE_PRIORITY,
                             APPS_CORE_SLEEP_TICK)
{
}

core::Thread::~Thread()
{
}

void core::Thread::setup()
{
    this->logInfo("thread started");
}

void core::Thread::loop()
{
}

void core::Thread::onHooked(uint32_t code)
{
    switch (code)
    {
    }
}