#ifndef __CORE_CORE_H
#define __CORE_CORE_H

#include "config.h"
#include "middlewares/thread/thread.h"
#include "apps/storage/storage.h"
#include "apps/shared/shared.h"
#include "apps/shared/utils/storage.h"

namespace core
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
        storage::Thread storage_th;
    };
}

#endif