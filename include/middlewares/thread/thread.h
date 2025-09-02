#ifndef __THREAD_THREAD_H
#define __THREAD_THREAD_H

#include <Arduino.h>
#include <cstdarg>
#include "guard.h"
#include "hooker.h"

namespace thread
{
    /**
     * @class Thread
     * @brief An abstract base class for creating and managing a dedicated FreeRTOS task.
     *
     * This class provides a C++ object-oriented wrapper around a FreeRTOS task.
     * To use it, inherit from this class and implement the pure virtual `setup()` and
     * `loop()` methods. The class ensures safe startup, shutdown, and state management
     * through a robust, thread-safe design. It is non-copyable and non-movable to
     * enforce correct resource ownership.
     */
    class Thread
    {
    public:
        /**
         * @brief The `Thread` class constructor.
         * @param name The name of the task, used for debugging.
         * @param stack_size The stack size to allocate for the task in bytes.
         * @param priority The priority of the task (higher number means higher priority).
         * @param sleep_tick The delay in ticks to wait at the end of each `loop()` iteration.
         */
        Thread(String name, uint32_t stack_size, UBaseType_t priority, TickType_t sleep_tick, TickType_t post_setup_sleep_tick = 200);

        /// @brief The `Thread` class destructor.
        virtual ~Thread();

        /// @brief Deleted copy constructor to prevent copying.
        Thread(const Thread &) = delete;

        /// @brief Deleted copy assignment operator to prevent copying.
        Thread &operator=(const Thread &) = delete;

        /// @brief Deleted move constructor to prevent moving.
        Thread(Thread &&) = delete;

        /// @brief Deleted move assignment operator to prevent moving.
        Thread &operator=(Thread &&) = delete;

        /**
         * @brief Starts the FreeRTOS task.
         *
         * Creates and starts the underlying task, which will first execute `setup()`
         * once, and then `loop()` indefinitely until stopped.
         *
         * @retval `true` if the task is successfully created, otherwise `false`.
         */
        bool start();

        /**
         * @brief Starts the FreeRTOS task and pins it to a specific CPU core.
         *
         * Creates and starts the underlying task, which will first execute `setup()`
         * once, and then `loop()` indefinitely until stopped.
         *
         * @param core_id The core to pin the task to (e.g., 0 or 1 on ESP32).
         * @retval `true` if the task is successfully created, otherwise `false`.
         */
        bool start(BaseType_t core_id);

        /**
         * @brief Suspends the running task.
         */
        void suspend();

        /**
         * @brief Resumes a suspended task.
         */
        void resume();

        /**
         * @brief Signals the task to gracefully stop execution.
         *
         * This will cause the task's main loop to exit. The destructor calls this
         * automatically and waits for completion.
         */
        void stop();

        /**
         * @brief Blocks lines after this function to let the threads run.
         */
        static void block();

    protected:
        /**
         * @brief User-defined setup code, executed once when the task starts.
         *
         * This method must be implemented by a derived class.
         */
        virtual void setup() = 0;

        /**
         * @brief User-defined loop code, executed repeatedly.
         *
         * This method must be implemented by a derived class. A delay of `sleep_tick`
         * is automatically added after each execution.
         */
        virtual void loop() = 0;

        /**
         * @brief User-defined function executed when the task is hooked.
         *
         * This method can be implemented optionally by a derived class. The code
         * parameter can be used for switching between several hook actions.
         *
         * @param code The code passed from the hooker.
         */
        virtual void onHooked(uint32_t code);

        /**
         * @brief User-defined function executed once when the thread is suspended.
         */
        virtual void onSuspended();

        /**
         * @brief User-defined function executed once when the thread is resumed.
         */
        virtual void onResumed();

        /**
         * @brief User-defined function executed once when the thread is stopped.
         */
        virtual void onStopped();

        /**
         * @brief Method for user to exit the thread from inside the thread.
         */
        void exit();

        /**
         * @brief Logs an informational message with a timestamp and thread name.
         * @param format The format string, followed by a variable number of arguments.
         */
        void logInfo(const char *format, ...);

        /**
         * @brief Logs a warning message with a timestamp and thread name.
         * @param format The format string, followed by a variable number of arguments.
         */
        void logWarn(const char *format, ...);

        /**
         * @brief Logs a critical error message with a timestamp and thread name.
         *  @param format The format string, followed by a variable number of arguments.
         */
        void logError(const char *format, ...);

        /**
         * @brief Logs a debug message with a timestamp and thread name.
         * @param format The format string, followed by a variable number of arguments.
         */
        void logDebug(const char *format, ...);

        /**
         * @brief Set the hooker instance to hook this thread.
         * @param hooker The hooker reference.
         * @param block If set to `true`, the thread will block until it is getting hooked. Defaults to `false`.
         * @retval `true` if success, otherwise `false`.
         */
        bool setHooker(Hooker &hooker, bool block = false);

    private:
        /// @brief Thread name passed to FreeRTOS API.
        String name;

        /// @brief Stack size allocated for the thread passed to FreeRTOS API.
        uint32_t stack_size;

        /// @brief Thread priority passed to FreeRTOS API.
        UBaseType_t priority;

        /// @brief Sleep tick of the thread.
        TickType_t sleep_tick;

        /// @brief Sleep tick after setup to let all the threads finish their setup first.
        TickType_t post_setup_sleep_tick;

        /// @brief Task handler from FreeRTOS API.
        TaskHandle_t handler;

        /// @brief A mutex to protect shared state variables.
        SemaphoreHandle_t mtx;

        /// @brief A mutex to protect logging.
        SemaphoreHandle_t log_mtx;

        /// @brief A binary to ensure the task is actually started.
        SemaphoreHandle_t start_sync;

        /// @brief A binary to ensure the task is actually stopped.
        SemaphoreHandle_t stop_sync;

        /// @brief Flag to initiate the serial once.
        static volatile bool serial_init;

        /// @brief The running state flag.
        bool running;

        /// @brief The suspended state flag.
        bool suspended;

        /// @brief The hooked state flag.
        bool hooked;

        /// @brief The hook blocked flag.
        bool hook_block;

        /// @brief Flag for initiation
        volatile bool is_init;

        /**
         * @brief Thread-safely sets the 'running' flag.
         * @param v The value to set.
         */
        void setRunning(bool v);

        /**
         * @brief Thread-safely sets the 'suspneded' flag.
         * @param v The value to set.
         */
        void setSuspended(bool v);

        /**
         * @brief Thread-safely sets the 'hooked' flag.
         * @param v The value to set.
         */
        void setHooked(bool v);

        /**
         * @brief Thread-safely sets the 'hook block` flag.
         * @param v The value to set.
         */
        void setHookBlock(bool v);

        /**
         * @brief Thread-safely gets the 'running' flag.
         * @retval The current value of the running flag.
         */
        bool getRunning();

        /**
         * @brief Thread-safely gets the 'suspended' flag.
         * @retval The current value of the suspended flag.
         */
        bool getSuspended();

        /**
         * @brief Thread-safely gets the 'hooked' flag.
         * @retval The current value of the hooked flag.
         */
        bool getHooked();

        /**
         * @brief Thread-safely gets the 'hook block' flag.
         * @retval The current value of the hook block flag.
         */
        bool getHookBlock();

        /**
         * @brief The static function wrapper passed to the FreeRTOS API.
         *
         * This function acts as the entry point for the task and calls the
         * object's instance methods.
         *
         * @param pvParameter A pointer to the `Thread` object instance.
         */
        static void task(void *pvParameter);
    };
}

#endif