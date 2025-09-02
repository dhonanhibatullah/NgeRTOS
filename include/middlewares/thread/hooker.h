#ifndef __THREAD_HOOK_H
#define __THREAD_HOOK_H

#include <Arduino.h>
#include "guard.h"

namespace thread
{
    /**
     * @class Hooker
     * @brief Provides a thread-safe mechanism to notify a FreeRTOS task.
     *
     * This class allows a task to register its handle once. Other tasks can then
     * use an instance of this class to send a lightweight FreeRTOS task
     * notification to the registered task without needing direct access to its handle.
     * It is designed to be a one-time hook; once a task handle is "grabbed", it
     * cannot be changed.
     */
    class Hooker
    {
    public:
        /// @brief The `Hooker` class constructor. Creates the internal mutex.
        Hooker();

        /// @brief The `Hooker` class destructor. Deletes the internal mutex.
        ~Hooker();

        /// @brief Deleted copy constructor to prevent copying.
        Hooker(const Hooker &) = delete;

        /// @brief Deleted copy assignment operator to prevent copying.
        Hooker &operator=(const Hooker &) = delete;

        /// @brief Deleted move constructor to prevent moving.
        Hooker(Hooker &&) = delete;

        /// @brief Deleted move assignment operator to prevent moving.
        Hooker &operator=(Hooker &&) = delete;

        /**
         * @brief Links a task handle to enable notifications.
         * @param task_handle The handle of the task to be notified. Must not be NULL.
         * @retval `true` if the handle was successfully set, otherwise `false`.
         */
        bool link(TaskHandle_t task_handle);

        /**
         * @brief Releases the set task handle.
         * @retval `true` if success, otherwise `false`.
         */
        bool release();

        /**
         * @brief Sends a notification to the set task.
         * @param code The 32-bit notification value to send to the task.
         * @retval `true` if success, otherwise `false`.
         */
        bool hook(uint32_t code);

    private:
        /// @brief Stores the handle of the task to be notified.
        TaskHandle_t handler;

        /// @brief Mutex for thread-safe access to class members.
        SemaphoreHandle_t mtx;

        /// @brief Flag to indicate if a task handle has been set.
        volatile bool is_set;
    };
}

#endif