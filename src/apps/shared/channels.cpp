#include "apps/shared/channels.h"

thread::Channel shared::storage_chan(1, sizeof(utils::StorageModel));
thread::Channel shared::web_chan(1, 2);