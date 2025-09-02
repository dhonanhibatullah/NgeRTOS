#ifndef __UTILS_STORAGE_H
#define __UTILS_STORAGE_H

#include <Arduino.h>
#include <Preferences.h>
#include <LittleFS.h>
#include "apps/shared/channels.h"

namespace utils
{
    enum StorageReturnType : uint8_t
    {
        STORAGE_OK,
        STORAGE_ERROR,
        STORAGE_TIMEOUT
    };

    enum StorageCommand : uint8_t
    {
        STORAGE_CMD_NONE,
        STORAGE_CMD_NVS_READ,
        STORAGE_CMD_NVS_WRITE,
        STORAGE_CMD_NVS_EXISTS,
        STORAGE_CMD_NVS_REMOVE,
        STORAGE_CMD_LFS_READ,
        STORAGE_CMD_LFS_WRITE,
        STORAGE_CMD_LFS_EXISTS,
        STORAGE_CMD_LFS_MKDIR,
        STORAGE_CMD_LFS_REMOVE,
        STORAGE_CMD_LFS_REMOVEDIR
    };

    struct StorageRW
    {
        char *path;
        void *value;
        size_t size;
        bool w;

        StorageRW(const char *path, void *buffer, size_t size);
        StorageRW(const char *path, const void *value, size_t size);
        ~StorageRW();
    };

    struct StoragePath
    {
        char *path;

        StoragePath(const char *path);
        ~StoragePath();
    };

    struct StorageModel
    {
        SemaphoreHandle_t lock;
        StorageCommand cmd;
        uint8_t *ack;
        void *data;

        StorageModel(void *data, StorageCommand cmd);
        ~StorageModel();
    };

    StorageReturnType nvsRead(const char *key, void *buffer, size_t size, TickType_t timeout = portMAX_DELAY);

    StorageReturnType nvsWrite(const char *key, const void *value, size_t size, TickType_t timeout = portMAX_DELAY);

    StorageReturnType nvsExists(const char *key, TickType_t timeout = portMAX_DELAY);

    StorageReturnType nvsRemove(const char *key, TickType_t timeout = portMAX_DELAY);

    StorageReturnType lfsRead(const char *path, void *buffer, size_t size, TickType_t timeout = portMAX_DELAY);

    StorageReturnType lfsWrite(const char *path, const void *value, size_t size, TickType_t timeout = portMAX_DELAY);

    StorageReturnType lfsExists(const char *path, TickType_t timeout = portMAX_DELAY);

    StorageReturnType lfsMkdir(const char *path, TickType_t timeout = portMAX_DELAY);

    StorageReturnType lfsRemove(const char *path, TickType_t timeout = portMAX_DELAY);

    StorageReturnType lfsRemoveDir(const char *path, TickType_t timeout = portMAX_DELAY);
}

#endif