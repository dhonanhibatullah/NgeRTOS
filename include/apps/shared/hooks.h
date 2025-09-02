#ifndef __SHARED_HOOKS_H
#define __SHARED_HOOKS_H

#include "middlewares/thread/hooker.h"

namespace shared
{
    extern thread::Hooker core_hook;
    extern thread::Hooker storage_hook;
    extern thread::Hooker web_hook;
}

#endif