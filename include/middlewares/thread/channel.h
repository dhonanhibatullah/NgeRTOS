#ifndef __THREAD_CHANNEL_H
#define __THREAD_CHANNEL_H

#include <Arduino.h>
#include "guard.h"

namespace thread
{
    /**
     * @class ChannelBind
     * @brief Represents a single channel listener bound to an Channel instance.
     */
    class ChannelBind
    {
    public:
        /// @brief Constructs a new, uninitialized ChannelBind object.
        ChannelBind();

        /**
         * @brief Constructs a new ChannelBind object with a specific queue handle and buffer length.
         * @param queue The FreeRTOS queue handle to which this listener is bound.
         * @param buffer_length The maximum number of items the queue can hold.
         */
        ChannelBind(QueueHandle_t queue, uint16_t buffer_length);

        /// @brief Destructor for the ChannelBind object.
        ~ChannelBind();

        /**
         * @brief Gets the number of messages currently waiting in the queue.
         * @retval The number of messages available to be received. Returns 0 if the queue is uninitialized.
         */
        uint16_t available();

        /**
         * @brief Gets the maximum number of items the queue can hold.
         * @retval The queue's buffer length.
         */
        uint16_t getBufferLength();

        /**
         * @brief Gets the underlying FreeRTOS queue handle.
         * @retval The `QueueHandle_t` used by this ChannelBind instance.
         */
        QueueHandle_t getQueueHandle();

        /**
         * @brief Receives a message from the queue.
         * @param buf A pointer to the buffer where the received data will be stored.
         * @param timeout The maximum time to wait for a message, in FreeRTOS ticks. Defaults to 0 (no wait).
         * @retval True if a message was successfully received, false otherwise.
         */
        bool receive(void *buf, TickType_t timeout = 0);

    private:
        ///  @brief The FreeRTOS queue handle associated with this listener.
        QueueHandle_t queue;

        /// @brief The maximum number of items this queue can hold.
        uint16_t buffer_length;
    };

    /**
     * @class Channel
     * @brief A thread-safe channel dispatcher that uses FreeRTOS queues for listeners.
     */
    class Channel
    {
    public:
        /**
         * @brief The `Channel` class constructor.
         * @param slot_num The maximum number of listener queues that can be bound to this channel.
         * @param item_size The size of item to be pushed to the channel.
         */
        Channel(uint16_t slot_num, size_t item_size);

        /// @brief The `Channel` class destructor.
        ~Channel();

        /// @brief Deleted copy constructor to make the class non-copyable.
        Channel(const Channel &) = delete;

        /// @brief Deleted copy assignment operator to make the class non-copyable.
        Channel &operator=(const Channel &) = delete;

        /**
         * @brief Binds a new listener by creating a FreeRTOS queue.
         * @param buffer_length The number of items the new queue can hold. Defaults to 1.
         * @param force Force push to the queue. Defaults to true.
         * @retval The `ChannelBind` class to handle the channel at the listener side.
         */
        ChannelBind bind(uint16_t buffer_length = 1, bool force = true);

        /**
         * @brief Unbinds a listener queue from the channel.
         * @param channel_bind The channel bind to unbind, previously returned by `bind()`.
         * @retval `true` if the queue was found and successfully unbound, `false` otherwise.
         */
        bool unbind(ChannelBind &channel_bind);

        /**
         * @brief Triggers the channel, sending data to all bound listeners.
         * @param data A pointer to the data to be sent to the listener queues.
         */
        void trigger(void *data);

    private:
        /**
         * @struct Slot
         * @brief Internal structure to manage a single listener registration.
         */
        struct Slot
        {
            bool occupied = false;
            bool force = false;
            QueueHandle_t queue = NULL;
        };

        /// @brief Mutex for ensuring thread-safe access to the slots.
        SemaphoreHandle_t mtx;

        /// @brief Pointer to the dynamically allocated array of listener slots.
        Slot *slots;

        /// @brief The total number of available slots.
        uint16_t slot_num;

        /// @brief The current number of occupied slots.
        uint16_t slot_cnt;

        /// @brief The number of size per item.
        size_t item_size;

        /// @brief Flag to denote whether the channel has been binded or not
        volatile bool is_binded;
    };
}

#endif