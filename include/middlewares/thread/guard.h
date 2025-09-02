#ifndef __THREAD_GUARD_H
#define __THREAD_GUARD_H

#include <Arduino.h>

namespace thread
{
    class Guard
    {
    public:
        /**
         * @brief The `Guard` class constructor. This is an RAII for guarding a thread-critical operation by taking a semaphore.
         * @param mtx The reference semaphore.
         */
        Guard(SemaphoreHandle_t mtx);

        /*
         * @brief The `Guard` class destructor. This is an RAII for releasing the semaphore at the end of a scope.
         */
        ~Guard();

    private:
        /// @brief  The reference mutex.
        SemaphoreHandle_t mtx;
    };
}

#endif