#include "middlewares/storage/nvs.h"

storage::NVS::NVS() : pref(nullptr),
                      mtx(NULL),
                      is_init(false)
{
}

storage::NVS::~NVS()
{
    storage::Guard ts(this->mtx);
    this->pref->end();
    delete this->pref;
    vSemaphoreDelete(this->mtx);
}

bool storage::NVS::init(const char *name)
{
    if (!this->is_init)
    {
        this->is_init = true;
        this->pref = new Preferences();
        this->mtx = xSemaphoreCreateMutex();
    }
    return this->pref->begin(name);
}

size_t storage::NVS::write(const char *key, char val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putChar(key, (int8_t)val);
}

size_t storage::NVS::write(const char *key, int8_t val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putChar(key, val);
}

size_t storage::NVS::write(const char *key, uint8_t val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putUChar(key, val);
}

size_t storage::NVS::write(const char *key, int16_t val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putShort(key, val);
}

size_t storage::NVS::write(const char *key, uint16_t val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putUShort(key, val);
}

size_t storage::NVS::write(const char *key, int32_t val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putInt(key, val);
}

size_t storage::NVS::write(const char *key, uint32_t val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putUInt(key, val);
}

size_t storage::NVS::write(const char *key, int64_t val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putLong64(key, val);
}

size_t storage::NVS::write(const char *key, uint64_t val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putULong64(key, val);
}

size_t storage::NVS::write(const char *key, float val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putFloat(key, val);
}

size_t storage::NVS::write(const char *key, double val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putDouble(key, val);
}

size_t storage::NVS::write(const char *key, const char *val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putString(key, val);
}

size_t storage::NVS::write(const char *key, String val)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->putString(key, val);
}

size_t storage::NVS::write(const char *key, const void *data, size_t size)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    if (!data)
        return 0;

    return this->pref->putBytes(key, data, size);
}

char storage::NVS::readChar(const char *key, char def)
{
    if (!this->is_init)
        return '\0';
    storage::Guard ts(this->mtx);

    return (char)this->pref->getChar(key, def);
}

int8_t storage::NVS::readInt8(const char *key, int8_t def)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getChar(key, def);
}

uint8_t storage::NVS::readUint8(const char *key, uint8_t def)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getUChar(key, def);
}

int16_t storage::NVS::readInt16(const char *key, int16_t def)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getShort(key, def);
}

uint16_t storage::NVS::readUint16(const char *key, uint16_t def)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getUShort(key, def);
}

int32_t storage::NVS::readInt32(const char *key, int32_t def)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getInt(key, def);
}

uint32_t storage::NVS::readUint32(const char *key, uint32_t def)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getUInt(key, def);
}

int64_t storage::NVS::readInt64(const char *key, int64_t def)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getLong64(key, def);
}

uint64_t storage::NVS::readUint64(const char *key, uint64_t def)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getULong64(key, def);
}

float storage::NVS::readFloat(const char *key, float def)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getFloat(key, def);
}

double storage::NVS::readDouble(const char *key, double def)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getDouble(key, def);
}

size_t storage::NVS::readString(const char *key, char *buf, size_t max_size)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    if (!buf)
        return 0;

    return this->pref->getString(key, buf, max_size);
}

String storage::NVS::readString(const char *key, String def)
{
    if (!this->is_init)
        return def;
    storage::Guard ts(this->mtx);

    return this->pref->getString(key, def);
}

size_t storage::NVS::read(const char *key, uint8_t *buf, size_t max_size)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    if (!buf)
        return 0;

    return this->pref->getBytes(key, buf, max_size);
}

bool storage::NVS::exists(const char *key)
{
    if (!this->is_init)
        return false;
    storage::Guard ts(this->mtx);

    return this->pref->isKey(key);
}

PreferenceType storage::NVS::getType(const char *key)
{
    if (!this->is_init)
        return PT_INVALID;
    storage::Guard ts(this->mtx);

    return this->pref->getType(key);
}

size_t storage::NVS::getSize(const char *key)
{
    if (!this->is_init)
        return 0;
    storage::Guard ts(this->mtx);

    return this->pref->getBytesLength(key);
}
