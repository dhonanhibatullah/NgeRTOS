#include "middlewares/thread/channel.h"

thread::ChannelBind::ChannelBind() : queue(NULL),
                                     buffer_length(0)
{
}

thread::ChannelBind::ChannelBind(
    QueueHandle_t queue,
    uint16_t buffer_length) : queue(queue),
                              buffer_length(buffer_length)
{
}

thread::ChannelBind::~ChannelBind()
{
}

uint16_t thread::ChannelBind::available()
{
    if (this->queue == NULL)
        return 0;

    return uxQueueMessagesWaiting(this->queue);
}

uint16_t thread::ChannelBind::getBufferLength()
{
    return this->buffer_length;
}

QueueHandle_t thread::ChannelBind::getQueueHandle()
{
    return this->queue;
}

bool thread::ChannelBind::receive(void *buf, TickType_t timeout)
{
    if (buf == nullptr || this->queue == NULL)
        return false;

    return (bool)(xQueueReceive(this->queue, buf, timeout) == pdTRUE);
}

thread::Channel::Channel(
    uint16_t slot_num,
    size_t item_size) : slot_num(slot_num),
                        slot_cnt(0),
                        item_size(item_size),
                        is_binded(false)
{
}

thread::Channel::~Channel()
{
    if (!this->is_binded)
        return;

    thread::Guard ts(this->mtx);

    for (uint16_t i = 0; i < this->slot_num; ++i)
    {
        if (this->slots[i].occupied)
            vQueueDelete(this->slots[i].queue);
    }
    delete[] this->slots;

    vSemaphoreDelete(this->mtx);
}

thread::ChannelBind thread::Channel::bind(uint16_t buffer_length, bool force)
{
    if (!this->is_binded)
    {
        this->is_binded = true;
        this->mtx = xSemaphoreCreateMutex();
        this->slots = new Slot[slot_num];
    }

    thread::Guard ts(this->mtx);

    if (this->slot_cnt == this->slot_num)
        return thread::ChannelBind();

    int id = 0;
    for (; id < this->slot_num; ++id)
    {
        if (!this->slots[id].occupied)
        {
            this->slots[id].occupied = true;
            this->slots[id].force = force;
            this->slots[id].queue = xQueueCreate(buffer_length, this->item_size);
            this->slot_cnt += 1;
            break;
        }
    }

    return thread::ChannelBind(this->slots[id].queue, buffer_length);
}

bool thread::Channel::unbind(thread::ChannelBind &channel_bind)
{
    if (!this->is_binded)
        return false;

    thread::Guard ts(this->mtx);

    for (uint16_t i = 0; i < this->slot_num; ++i)
    {
        if (this->slots[i].occupied && this->slots[i].queue == channel_bind.getQueueHandle())
        {
            this->slots[i].occupied = false;
            this->slots[i].queue = NULL;
            vQueueDelete(channel_bind.getQueueHandle());
            this->slot_cnt -= 1;
            return true;
        }
    }

    return false;
}

void thread::Channel::trigger(void *data)
{
    if (!this->is_binded)
        return;

    thread::Guard ts(this->mtx);

    for (int i = 0; i < this->slot_num; ++i)
    {
        if (this->slots[i].occupied)
        {
            if ((xQueueSend(this->slots[i].queue, data, 0) != pdTRUE) && this->slots[i].force)
            {
                void *tmp = malloc(this->item_size);
                xQueueReceive(this->slots[i].queue, tmp, 0);
                xQueueSend(this->slots[i].queue, data, 0);
                free(tmp);
            }
        }
    }
}