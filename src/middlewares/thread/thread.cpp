#include "middlewares/thread/thread.h"

volatile bool thread::Thread::serial_init = false;

thread::Thread::Thread(
    String name,
    uint32_t stack_size,
    UBaseType_t priority,
    TickType_t sleep_tick,
    TickType_t post_setup_sleep_tick) : name(name),
                                        stack_size(stack_size),
                                        priority(priority),
                                        sleep_tick(sleep_tick),
                                        handler(NULL),
                                        running(false),
                                        suspended(false),
                                        hooked(false),
                                        hook_block(false),
                                        is_init(false),
                                        post_setup_sleep_tick(post_setup_sleep_tick)
{
    if (!thread::Thread::serial_init)
    {
        thread::Thread::serial_init = true;
        Serial.begin(115200);
    }
}

thread::Thread::~Thread()
{
    this->stop();
    vSemaphoreDelete(this->mtx);
    vSemaphoreDelete(this->start_sync);
    vSemaphoreDelete(this->stop_sync);
}

bool thread::Thread::start()
{
    if (!this->is_init)
    {
        this->is_init = true;
        this->mtx = xSemaphoreCreateMutex();
        this->log_mtx = xSemaphoreCreateMutex();
        this->start_sync = xSemaphoreCreateBinary();
        this->stop_sync = xSemaphoreCreateBinary();
    }

    if (this->getRunning())
        return false;

    this->setSuspended(false);
    this->setHooked(false);
    this->setHookBlock(false);

    if (xTaskCreate(
            Thread::task,
            this->name.c_str(),
            this->stack_size,
            this,
            this->priority,
            &this->handler) != pdPASS)
        return false;

    return xSemaphoreTake(this->start_sync, portMAX_DELAY) == pdTRUE;
}

bool thread::Thread::start(BaseType_t core_id)
{
    if (this->getRunning())
        return false;

    this->setSuspended(false);
    this->setHooked(false);
    this->setHookBlock(false);

    if (xTaskCreatePinnedToCore(
            Thread::task,
            this->name.c_str(),
            this->stack_size,
            this,
            this->priority,
            &this->handler,
            core_id) != pdPASS)
        return false;

    return xSemaphoreTake(this->start_sync, portMAX_DELAY) == pdTRUE;
}

void thread::Thread::suspend()
{
    if (!this->getSuspended())
    {
        vTaskSuspend(this->handler);
        this->setSuspended(true);

        this->onSuspended();
    }
}

void thread::Thread::resume()
{
    if (this->getSuspended())
    {
        vTaskResume(this->handler);
        this->setSuspended(false);

        this->onResumed();
    }
}

void thread::Thread::stop()
{
    if (!this->getRunning())
        return;

    this->setRunning(false);
    xSemaphoreTake(this->stop_sync, portMAX_DELAY);
    this->handler = NULL;

    this->onStopped();
}

void thread::Thread::onHooked(uint32_t code)
{
}

void thread::Thread::onSuspended()
{
}

void thread::Thread::onResumed()
{
}

void thread::Thread::onStopped()
{
}

bool thread::Thread::setHooker(Hooker &hooker, bool block)
{
    if (hooker.link(this->handler))
    {
        this->setHooked(true);
        this->setHookBlock(block);
        return true;
    }
    return false;
}

void thread::Thread::setRunning(bool v)
{
    thread::Guard ts(this->mtx);
    this->running = v;
}

void thread::Thread::setSuspended(bool v)
{
    thread::Guard ts(this->mtx);
    this->suspended = v;
}

void thread::Thread::setHooked(bool v)
{
    thread::Guard ts(this->mtx);
    this->hooked = v;
}

void thread::Thread::setHookBlock(bool v)
{
    thread::Guard ts(this->mtx);
    this->hook_block = v;
}

bool thread::Thread::getRunning()
{
    thread::Guard ts(this->mtx);
    return this->running;
}

bool thread::Thread::getSuspended()
{
    thread::Guard ts(this->mtx);
    return this->suspended;
}

bool thread::Thread::getHooked()
{
    thread::Guard ts(this->mtx);
    return this->hooked;
}

bool thread::Thread::getHookBlock()
{
    thread::Guard ts(this->mtx);
    return this->hook_block;
}

void thread::Thread::logInfo(const char *format, ...)
{
#ifdef LOG_INFO_ENABLE
    static const char fmt[] = "%7u.%03u [INFO ] (%s): ";

    thread::Guard ts(this->log_mtx);

    unsigned long now_ts = millis();
    unsigned long sec_ts = now_ts / 1000;
    unsigned long msec_ts = now_ts % 1000;

    size_t prefix_len = snprintf(nullptr, 0, fmt, sec_ts, msec_ts, this->name.c_str());

    va_list args;
    va_start(args, format);
    size_t user_message_len = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (user_message_len < 0)
        return;

    size_t buffer_size = prefix_len + user_message_len + 2;
    char *log_buffer = new char[buffer_size];

    va_start(args, format);
    snprintf(log_buffer, buffer_size, fmt, sec_ts, msec_ts, this->name.c_str());
    vsnprintf(log_buffer + prefix_len, buffer_size - prefix_len, format, args);
    va_end(args);

    log_buffer[buffer_size - 2] = '\n';
    log_buffer[buffer_size - 1] = '\0';

    printf("%s", log_buffer);
    delete[] log_buffer;
#endif
}

void thread::Thread::logWarn(const char *format, ...)
{
#ifdef LOG_WARN_ENABLE
    static const char fmt[] = "%7u.%03u [WARN ] (%s): ";

    thread::Guard ts(this->log_mtx);

    unsigned long now_ts = millis();
    unsigned long sec_ts = now_ts / 1000;
    unsigned long msec_ts = now_ts % 1000;

    size_t prefix_len = snprintf(nullptr, 0, fmt, sec_ts, msec_ts, this->name.c_str());

    va_list args;
    va_start(args, format);
    size_t user_message_len = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (user_message_len < 0)
        return;

    size_t buffer_size = prefix_len + user_message_len + 2;
    char *log_buffer = new char[buffer_size];

    va_start(args, format);
    snprintf(log_buffer, buffer_size, fmt, sec_ts, msec_ts, this->name.c_str());
    vsnprintf(log_buffer + prefix_len, buffer_size - prefix_len, format, args);
    va_end(args);

    log_buffer[buffer_size - 2] = '\n';
    log_buffer[buffer_size - 1] = '\0';

    printf("%s", log_buffer);
    delete[] log_buffer;
#endif
}

void thread::Thread::logError(const char *format, ...)
{
#ifdef LOG_ERROR_ENABLE
    static const char fmt[] = "%7u.%03u [ERROR] (%s): ";

    thread::Guard ts(this->log_mtx);

    unsigned long now_ts = millis();
    unsigned long sec_ts = now_ts / 1000;
    unsigned long msec_ts = now_ts % 1000;

    size_t prefix_len = snprintf(nullptr, 0, fmt, sec_ts, msec_ts, this->name.c_str());

    va_list args;
    va_start(args, format);
    size_t user_message_len = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (user_message_len < 0)
        return;

    size_t buffer_size = prefix_len + user_message_len + 2;
    char *log_buffer = new char[buffer_size];

    va_start(args, format);
    snprintf(log_buffer, buffer_size, fmt, sec_ts, msec_ts, this->name.c_str());
    vsnprintf(log_buffer + prefix_len, buffer_size - prefix_len, format, args);
    va_end(args);

    log_buffer[buffer_size - 2] = '\n';
    log_buffer[buffer_size - 1] = '\0';

    printf("%s", log_buffer);
    delete[] log_buffer;
#endif
}

void thread::Thread::logDebug(const char *format, ...)
{
#ifdef LOG_DEBUG_ENABLE
    static const char fmt[] = "%7u.%03u [DEBUG] (%s): ";

    thread::Guard ts(this->log_mtx);

    unsigned long now_ts = millis();
    unsigned long sec_ts = now_ts / 1000;
    unsigned long msec_ts = now_ts % 1000;

    size_t prefix_len = snprintf(nullptr, 0, fmt, sec_ts, msec_ts, this->name.c_str());

    va_list args;
    va_start(args, format);
    size_t user_message_len = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (user_message_len < 0)
        return;

    size_t buffer_size = prefix_len + user_message_len + 2;
    char *log_buffer = new char[buffer_size];

    va_start(args, format);
    snprintf(log_buffer, buffer_size, fmt, sec_ts, msec_ts, this->name.c_str());
    vsnprintf(log_buffer + prefix_len, buffer_size - prefix_len, format, args);
    va_end(args);

    log_buffer[buffer_size - 2] = '\n';
    log_buffer[buffer_size - 1] = '\0';

    printf("%s", log_buffer);
    delete[] log_buffer;
#endif
}

void thread::Thread::task(void *pvParameter)
{
    Thread *self = (Thread *)pvParameter;
    uint32_t hook_code;

    self->setup();
    self->setRunning(true);
    xSemaphoreGive(self->start_sync);
    vTaskDelay(self->post_setup_sleep_tick);

    while (self->getRunning())
    {
        if (self->getHooked())
            if (xTaskNotifyWait(0, 0, &hook_code, ((self->getHookBlock()) ? portMAX_DELAY : 0)) == pdTRUE)
                self->onHooked(hook_code);

        self->loop();
        vTaskDelay(self->sleep_tick);
    }

    xSemaphoreGive(self->stop_sync);
    vTaskDelete(NULL);
}