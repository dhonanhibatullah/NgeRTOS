#include "apps/storage/storage.h"

storage::Thread::Thread() : thread::Thread(
                                APPS_STORAGE_NAME,
                                APPS_STORAGE_STACK_SIZE,
                                APPS_STORAGE_PRIORITY,
                                APPS_STORAGE_SLEEP_TICK)
{
}

storage::Thread::~Thread()
{
    shared::storage_hook.release();
    shared::storage_chan.unbind(this->channel_bind);
    this->pref->end();
    this->lfs->end();
    delete this->pref;
}

void storage::Thread::setup()
{
    this->logInfo("thread started");
    this->setHooker(shared::storage_hook);
    this->channel_bind = shared::storage_chan.bind(APPS_STORAGE_BUFFER, false);
    this->channel_buf = new utils::StorageModel(nullptr, utils::STORAGE_CMD_NONE);

    this->pref = new Preferences();
    if (!this->pref->begin(APPS_STORAGE_NAME))
    {
        this->logError("NVS failed to initiate, exitting thread...");
        this->exit();
    }
    this->logInfo("NVS initiated successfully");

    this->lfs = &LittleFS;
    if (!this->lfs->begin(true))
    {
        this->logError("LFS failed to initiate, exitting thread...");
        this->exit();
    }
    this->logInfo("LFS initiated successfully");
}

void storage::Thread::loop()
{
    if (this->channel_bind.receive(this->channel_buf, portMAX_DELAY))
    {
        switch (this->channel_buf->cmd)
        {
        case utils::STORAGE_CMD_NVS_READ:
            this->nvsReadCb();
            break;
        case utils::STORAGE_CMD_NVS_WRITE:
            this->nvsWriteCb();
            break;
        case utils::STORAGE_CMD_NVS_EXISTS:
            this->nvsExistsCb();
            break;
        case utils::STORAGE_CMD_NVS_REMOVE:
            this->nvsRemoveCb();
            break;
        case utils::STORAGE_CMD_LFS_READ:
            this->lfsReadCb();
            break;
        case utils::STORAGE_CMD_LFS_WRITE:
            this->lfsWriteCb();
            break;
        case utils::STORAGE_CMD_LFS_EXISTS:
            this->lfsExistsCb();
            break;
        case utils::STORAGE_CMD_LFS_MKDIR:
            this->lfsMkdirCb();
            break;
        case utils::STORAGE_CMD_LFS_REMOVE:
            this->lfsRemoveCb();
            break;
        case utils::STORAGE_CMD_LFS_REMOVEDIR:
            this->lfsRemoveDirCb();
            break;
        }
    }
}

void storage::Thread::onHooked(uint32_t code)
{
    switch (code)
    {
    }
}

void storage::Thread::onSuspended()
{
    this->logInfo("thread suspended");
}

void storage::Thread::onResumed()
{
    this->logInfo("thread resumed");
}

void storage::Thread::onStopped()
{
    this->logInfo("thread stopped");
}

void storage::Thread::nvsReadCb()
{
    utils::StorageRW *m = (utils::StorageRW *)this->channel_buf->data;
    *this->channel_buf->ack = 0;

    if (this->pref->getBytes(m->path, m->value, m->size) > 0)
        *this->channel_buf->ack = 1;

    xSemaphoreGive(this->channel_buf->lock);
}

void storage::Thread::nvsWriteCb()
{
    utils::StorageRW *m = (utils::StorageRW *)this->channel_buf->data;
    *this->channel_buf->ack = 0;

    if (this->pref->putBytes(m->path, m->value, m->size) > 0)
        *this->channel_buf->ack = 1;

    xSemaphoreGive(this->channel_buf->lock);
}

void storage::Thread::nvsExistsCb()
{
    utils::StoragePath *m = (utils::StoragePath *)this->channel_buf->data;
    *this->channel_buf->ack = 0;

    if (this->pref->isKey(m->path))
        *this->channel_buf->ack = 1;

    xSemaphoreGive(this->channel_buf->lock);
}

void storage::Thread::nvsRemoveCb()
{
    utils::StoragePath *m = (utils::StoragePath *)this->channel_buf->data;
    *this->channel_buf->ack = 0;

    if (this->pref->remove(m->path))
        *this->channel_buf->ack = 1;

    xSemaphoreGive(this->channel_buf->lock);
}

void storage::Thread::lfsReadCb()
{
    utils::StorageRW *m = (utils::StorageRW *)this->channel_buf->data;
    *this->channel_buf->ack = 0;

    fs::File file = this->lfs->open(m->path, FILE_READ);
    if (file)
    {
        if (file.read((uint8_t *)m->value, m->size) > 0)
            *this->channel_buf->ack = 1;
        file.close();
    }

    xSemaphoreGive(this->channel_buf->lock);
}

void storage::Thread::lfsWriteCb()
{
    utils::StorageRW *m = (utils::StorageRW *)this->channel_buf->data;
    *this->channel_buf->ack = 0;

    fs::File file = this->lfs->open(m->path, FILE_WRITE);
    if (file)
    {
        if (file.write((uint8_t *)m->value, m->size) > 0)
            *this->channel_buf->ack = 1;
        file.close();
    }

    xSemaphoreGive(this->channel_buf->lock);
}

void storage::Thread::lfsExistsCb()
{
    utils::StoragePath *m = (utils::StoragePath *)this->channel_buf->data;
    *this->channel_buf->ack = 0;

    if (this->lfs->exists(m->path))
        *this->channel_buf->ack = 1;

    xSemaphoreGive(this->channel_buf->lock);
}

void storage::Thread::lfsMkdirCb()
{
    utils::StoragePath *m = (utils::StoragePath *)this->channel_buf->data;
    *this->channel_buf->ack = 0;

    if (this->lfs->mkdir(m->path))
        *this->channel_buf->ack = 1;

    xSemaphoreGive(this->channel_buf->lock);
}

void storage::Thread::lfsRemoveCb()
{
    utils::StoragePath *m = (utils::StoragePath *)this->channel_buf->data;
    *this->channel_buf->ack = 0;

    if (this->lfs->remove(m->path))
        *this->channel_buf->ack = 1;

    xSemaphoreGive(this->channel_buf->lock);
}

void storage::Thread::lfsRemoveDirCb()
{
    utils::StoragePath *m = (utils::StoragePath *)this->channel_buf->data;
    *this->channel_buf->ack = 0;

    if (this->lfs->rmdir(m->path))
        *this->channel_buf->ack = 1;

    xSemaphoreGive(this->channel_buf->lock);
}
