#include "apps/shared/utils/storage.h"

utils::StorageRW::StorageRW(const char *path, void *value, size_t size) : value(value), size(size), w(false)
{
    this->path = (char *)malloc((strlen(path) + 1) * sizeof(char));
    strcpy(this->path, path);
}

utils::StorageRW::StorageRW(const char *path, const void *value, size_t size) : size(size), w(true)
{
    this->path = (char *)malloc((strlen(path) + 1) * sizeof(char));
    strcpy(this->path, path);
    this->value = malloc(size);
    memcpy(this->value, value, size);
}

utils::StorageRW::~StorageRW()
{
    free(this->path);
    if (w)
        free(this->value);
}

utils::StoragePath::StoragePath(const char *path)
{
    this->path = (char *)malloc((strlen(path) + 1) * sizeof(char));
    strcpy(this->path, path);
}

utils::StoragePath::~StoragePath()
{
    free(this->path);
}

utils::StorageModel::StorageModel(void *data, utils::StorageCommand cmd) : data(data), cmd(cmd)
{
    this->lock = xSemaphoreCreateBinary();
    this->ack = new uint8_t;
}

utils::StorageModel::~StorageModel()
{
    delete this->ack;
    vSemaphoreDelete(this->lock);
}

utils::StorageReturnType utils::nvsRead(const char *key, void *buffer, size_t size, TickType_t timeout)
{
    if (key == nullptr || buffer == nullptr || size == 0)
        return utils::STORAGE_ERROR;

    utils::StorageReturnType ret;
    utils::StorageRW data(key, buffer, size);
    utils::StorageModel msg(&data, utils::STORAGE_CMD_NVS_READ);
    shared::storage_chan.trigger(&msg);

    if (xSemaphoreTake(msg.lock, timeout) == pdTRUE)
    {
        if (*msg.ack == 0)
            ret = utils::STORAGE_ERROR;
        else if (*msg.ack == 1)
            ret = utils::STORAGE_OK;
    }
    else
        ret = utils::STORAGE_TIMEOUT;

    return ret;
}

utils::StorageReturnType utils::nvsWrite(const char *key, const void *value, size_t size, TickType_t timeout)
{
    if (key == nullptr || value == nullptr || size == 0)
        return utils::STORAGE_ERROR;

    utils::StorageReturnType ret;
    utils::StorageRW data(key, value, size);
    utils::StorageModel msg(&data, utils::STORAGE_CMD_NVS_WRITE);
    shared::storage_chan.trigger(&msg);

    if (xSemaphoreTake(msg.lock, timeout) == pdTRUE)
    {
        if (*msg.ack == 0)
            ret = utils::STORAGE_ERROR;
        else if (*msg.ack == 1)
            ret = utils::STORAGE_OK;
    }
    else
        ret = utils::STORAGE_TIMEOUT;

    return ret;
}

utils::StorageReturnType utils::nvsExists(const char *key, TickType_t timeout)
{
    if (key == nullptr)
        return utils::STORAGE_ERROR;

    utils::StorageReturnType ret;
    utils::StoragePath data(key);
    utils::StorageModel msg(&data, utils::STORAGE_CMD_NVS_EXISTS);
    shared::storage_chan.trigger(&msg);

    if (xSemaphoreTake(msg.lock, timeout) == pdTRUE)
    {
        if (*msg.ack == 0)
            ret = utils::STORAGE_ERROR;
        else if (*msg.ack == 1)
            ret = utils::STORAGE_OK;
    }
    else
        ret = utils::STORAGE_TIMEOUT;

    return ret;
}

utils::StorageReturnType utils::nvsRemove(const char *key, TickType_t timeout)
{
    if (key == nullptr)
        return utils::STORAGE_ERROR;

    utils::StorageReturnType ret;
    utils::StoragePath data(key);
    utils::StorageModel msg(&data, utils::STORAGE_CMD_NVS_REMOVE);
    shared::storage_chan.trigger(&msg);

    if (xSemaphoreTake(msg.lock, timeout) == pdTRUE)
    {
        if (*msg.ack == 0)
            ret = utils::STORAGE_ERROR;
        else if (*msg.ack == 1)
            ret = utils::STORAGE_OK;
    }
    else
        ret = utils::STORAGE_TIMEOUT;

    return ret;
}

utils::StorageReturnType utils::lfsRead(const char *path, void *buffer, size_t size, TickType_t timeout)
{
    if (path == nullptr || buffer == nullptr || size == 0)
        return utils::STORAGE_ERROR;

    utils::StorageReturnType ret;
    utils::StorageRW data(path, buffer, size);
    utils::StorageModel msg(&data, utils::STORAGE_CMD_LFS_READ);
    shared::storage_chan.trigger(&msg);

    if (xSemaphoreTake(msg.lock, timeout) == pdTRUE)
    {
        if (*msg.ack == 0)
            ret = utils::STORAGE_ERROR;
        else if (*msg.ack == 1)
            ret = utils::STORAGE_OK;
    }
    else
        ret = utils::STORAGE_TIMEOUT;

    return ret;
}

utils::StorageReturnType utils::lfsWrite(const char *path, const void *value, size_t size, TickType_t timeout)
{
    if (path == nullptr || value == nullptr || size == 0)
        return utils::STORAGE_ERROR;

    utils::StorageReturnType ret;
    utils::StorageRW data(path, value, size);
    utils::StorageModel msg(&data, utils::STORAGE_CMD_LFS_WRITE);
    shared::storage_chan.trigger(&msg);

    if (xSemaphoreTake(msg.lock, timeout) == pdTRUE)
    {
        if (*msg.ack == 0)
            ret = utils::STORAGE_ERROR;
        else if (*msg.ack == 1)
            ret = utils::STORAGE_OK;
    }
    else
        ret = utils::STORAGE_TIMEOUT;

    return ret;
}

utils::StorageReturnType utils::lfsExists(const char *path, TickType_t timeout)
{
    if (path == nullptr)
        return utils::STORAGE_ERROR;

    utils::StorageReturnType ret;
    utils::StoragePath data(path);
    utils::StorageModel msg(&data, utils::STORAGE_CMD_LFS_EXISTS);
    shared::storage_chan.trigger(&msg);

    if (xSemaphoreTake(msg.lock, timeout) == pdTRUE)
    {
        if (*msg.ack == 0)
            ret = utils::STORAGE_ERROR;
        else if (*msg.ack == 1)
            ret = utils::STORAGE_OK;
    }
    else
        ret = utils::STORAGE_TIMEOUT;

    return ret;
}

utils::StorageReturnType utils::lfsMkdir(const char *path, TickType_t timeout)
{
    if (path == nullptr)
        return utils::STORAGE_ERROR;

    utils::StorageReturnType ret;
    utils::StoragePath data(path);
    utils::StorageModel msg(&data, utils::STORAGE_CMD_LFS_MKDIR);
    shared::storage_chan.trigger(&msg);

    if (xSemaphoreTake(msg.lock, timeout) == pdTRUE)
    {
        if (*msg.ack == 0)
            ret = utils::STORAGE_ERROR;
        else if (*msg.ack == 1)
            ret = utils::STORAGE_OK;
    }
    else
        ret = utils::STORAGE_TIMEOUT;

    return ret;
}

utils::StorageReturnType utils::lfsRemove(const char *path, TickType_t timeout)
{
    if (path == nullptr)
        return utils::STORAGE_ERROR;

    utils::StorageReturnType ret;
    utils::StoragePath data(path);
    utils::StorageModel msg(&data, utils::STORAGE_CMD_LFS_REMOVE);
    shared::storage_chan.trigger(&msg);

    if (xSemaphoreTake(msg.lock, timeout) == pdTRUE)
    {
        if (*msg.ack == 0)
            ret = utils::STORAGE_ERROR;
        else if (*msg.ack == 1)
            ret = utils::STORAGE_OK;
    }
    else
        ret = utils::STORAGE_TIMEOUT;

    return ret;
}

utils::StorageReturnType utils::lfsRemoveDir(const char *path, TickType_t timeout)
{
    if (path == nullptr)
        return utils::STORAGE_ERROR;

    utils::StorageReturnType ret;
    utils::StoragePath data(path);
    utils::StorageModel msg(&data, utils::STORAGE_CMD_LFS_REMOVEDIR);
    shared::storage_chan.trigger(&msg);

    if (xSemaphoreTake(msg.lock, timeout) == pdTRUE)
    {
        if (*msg.ack == 0)
            ret = utils::STORAGE_ERROR;
        else if (*msg.ack == 1)
            ret = utils::STORAGE_OK;
    }
    else
        ret = utils::STORAGE_TIMEOUT;

    return ret;
}
