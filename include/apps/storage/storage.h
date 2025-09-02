#ifndef __STORAGE_STORAGE_H
#define __STORAGE_STORAGE_H

#include <Preferences.h>
#include <LittleFS.h>
#include "config.h"
#include "middlewares/thread/thread.h"
#include "middlewares/thread/channel.h"
#include "apps/shared/shared.h"
#include "apps/shared/utils/storage.h"

namespace storage
{
    class Thread : public thread::Thread
    {
    public:
        Thread();
        ~Thread();
        Thread(const Thread &) = delete;
        Thread &operator=(const Thread &) = delete;

    protected:
        void setup() override;
        void loop() override;
        void onHooked(uint32_t code) override;
        void onSuspended() override;
        void onResumed() override;
        void onStopped() override;

    private:
        thread::ChannelBind channel_bind;
        utils::StorageModel *channel_buf;
        Preferences *pref;
        fs::LittleFSFS *lfs;
        void nvsReadCb();
        void nvsWriteCb();
        void nvsExistsCb();
        void nvsRemoveCb();
        void lfsReadCb();
        void lfsWriteCb();
        void lfsExistsCb();
        void lfsMkdirCb();
        void lfsRemoveCb();
        void lfsRemoveDirCb();
    };
}

#endif