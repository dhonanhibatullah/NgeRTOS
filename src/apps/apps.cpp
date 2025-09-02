#include "apps/apps.h"

void apps::run()
{
    core::Thread core;
    core.start();
    thread::Thread::block();
}