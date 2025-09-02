#ifndef __SHARED_CHANNELS_H
#define __SHARED_CHANNELS_H

#include "middlewares/thread/channel.h"
#include "utils/storage.h"

namespace shared
{
    extern thread::Channel storage_chan;
    extern thread::Channel web_chan;
}

#endif