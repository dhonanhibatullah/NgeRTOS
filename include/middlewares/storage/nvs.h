#ifndef __STORAGE_NVS_H
#define __STORAGE_NVS_H

#include <Preferences.h>
#include "guard.h"

namespace storage
{
    class NVS
    {
    public:
        /// @brief Constructs the NVS wrapper, creating the Preferences object and a mutex.
        NVS();

        /// @brief Destructor for the NVS wrapper. It closes the Preferences and deletes the mutex.
        ~NVS();

        /// @brief Deleted copy constructor to prevent copying of the NVS object.
        NVS(const NVS &) = delete;

        /// @brief Deleted assignment operator to prevent assignment of the NVS object.
        NVS &operator=(const NVS &) = delete;

        /**
         * @brief Initializes the NVS wrapper by beginning a new Preferences namespace.
         * @param name The namespace name (max 15 chars) to be opened for read/write.
         * @retval true if the namespace was opened successfully, false otherwise.
         */
        bool init(const char *name);

        /**
         * @brief Writes a char value to the specified key in the currently open namespace.
         * @param key The key to write the value to.
         * @param val The char value to be written.
         * @retval The number of bytes written (1) or 0 on failure.
         */
        size_t write(const char *key, char val);

        /**
         * @brief Writes an int8_t value to the specified key.
         * @param key The key to write the value to.
         * @param val The int8_t value to be written.
         * @retval The number of bytes written (1) or 0 on failure.
         */
        size_t write(const char *key, int8_t val);

        /**
         * @brief Writes a uint8_t value to the specified key.
         * @param key The key to write the value to.
         * @param val The uint8_t value to be written.
         * @retval The number of bytes written (1) or 0 on failure.
         */
        size_t write(const char *key, uint8_t val);

        /**
         * @brief Writes an int16_t value to the specified key.
         * @param key The key to write the value to.
         * @param val The int16_t value to be written.
         * @retval The number of bytes written (2) or 0 on failure.
         */
        size_t write(const char *key, int16_t val);

        /**
         * @brief Writes a uint16_t value to the specified key.
         * @param key The key to write the value to.
         * @param val The uint16_t value to be written.
         * @retval The number of bytes written (2) or 0 on failure.
         */
        size_t write(const char *key, uint16_t val);

        /**
         * @brief Writes an int32_t value to the specified key.
         * @param key The key to write the value to.
         * @param val The int32_t value to be written.
         * @retval The number of bytes written (4) or 0 on failure.
         */
        size_t write(const char *key, int32_t val);

        /**
         * @brief Writes a uint32_t value to the specified key.
         * @param key The key to write the value to.
         * @param val The uint32_t value to be written.
         * @retval The number of bytes written (4) or 0 on failure.
         */
        size_t write(const char *key, uint32_t val);

        /**
         * @brief Writes an int64_t value to the specified key.
         * @param key The key to write the value to.
         * @param val The int64_t value to be written.
         * @retval The number of bytes written (8) or 0 on failure.
         */
        size_t write(const char *key, int64_t val);

        /**
         * @brief Writes a uint64_t value to the specified key.
         * @param key The key to write the value to.
         * @param val The uint64_t value to be written.
         * @retval The number of bytes written (8) or 0 on failure.
         */
        size_t write(const char *key, uint64_t val);

        /**
         * @brief Writes a float value to the specified key.
         * @param key The key to write the value to.
         * @param val The float value to be written.
         * @retval The number of bytes written (4) or 0 on failure.
         */
        size_t write(const char *key, float val);

        /**
         * @brief Writes a double value to the specified key.
         * @param key The key to write the value to.
         * @param val The double value to be written.
         * @retval The number of bytes written (8) or 0 on failure.
         */
        size_t write(const char *key, double val);

        /**
         * @brief Writes a C-style string to the specified key.
         * @param key The key to write the string to.
         * @param val The C-style string to be written.
         * @retval The number of bytes written or 0 on failure.
         */
        size_t write(const char *key, const char *val);

        /**
         * @brief Writes an Arduino String object to the specified key.
         * @param key The key to write the string to.
         * @param val The Arduino String object to be written.
         * @retval The number of bytes written or 0 on failure.
         */
        size_t write(const char *key, String val);

        /**
         * @brief Writes a binary blob of data to the specified key.
         * @param key The key to write the data to.
         * @param data A pointer to the data to be written.
         * @param size The size of the data in bytes.
         * @retval The number of bytes written or 0 on failure.
         */
        size_t write(const char *key, const void *data, size_t size);

        /**
         * @brief Reads a char value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The char value read from NVS, or the default value if the key doesn't exist.
         */
        char readChar(const char *key, char def = '\0');

        /**
         * @brief Reads an int8_t value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The int8_t value read from NVS, or the default value if the key doesn't exist.
         */
        int8_t readInt8(const char *key, int8_t def = 0);

        /**
         * @brief Reads a uint8_t value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The uint8_t value read from NVS, or the default value if the key doesn't exist.
         */
        uint8_t readUint8(const char *key, uint8_t def = 0);

        /**
         * @brief Reads an int16_t value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The int16_t value read from NVS, or the default value if the key doesn't exist.
         */
        int16_t readInt16(const char *key, int16_t def = 0);

        /**
         * @brief Reads a uint16_t value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The uint16_t value read from NVS, or the default value if the key doesn't exist.
         */
        uint16_t readUint16(const char *key, uint16_t def = 0);

        /**
         * @brief Reads an int32_t value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The int32_t value read from NVS, or the default value if the key doesn't exist.
         */
        int32_t readInt32(const char *key, int32_t def = 0);

        /**
         * @brief Reads a uint32_t value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The uint32_t value read from NVS, or the default value if the key doesn't exist.
         */
        uint32_t readUint32(const char *key, uint32_t def = 0);

        /**
         * @brief Reads an int64_t value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The int64_t value read from NVS, or the default value if the key doesn't exist.
         */
        int64_t readInt64(const char *key, int64_t def = 0);

        /**
         * @brief Reads a uint64_t value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The uint64_t value read from NVS, or the default value if the key doesn't exist.
         */
        uint64_t readUint64(const char *key, uint64_t def = 0);

        /**
         * @brief Reads a float value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The float value read from NVS, or the default value if the key doesn't exist.
         */
        float readFloat(const char *key, float def = 0);

        /**
         * @brief Reads a double value from the specified key.
         * @param key The key to read from.
         * @param def The default value to return if the key is not found.
         * @retval The double value read from NVS, or the default value if the key doesn't exist.
         */
        double readDouble(const char *key, double def = 0);

        /**
         * @brief Reads a C-style string into a buffer from the specified key.
         * @param key The key to read the string from.
         * @param buf A pointer to the character buffer to store the string.
         * @param max_size The maximum size of the buffer.
         * @retval The number of bytes read, excluding the null-terminator.
         */
        size_t readString(const char *key, char *buf, size_t max_size);

        /**
         * @brief Reads an Arduino String object from the specified key.
         * @param key The key to read from.
         * @param def The default String object to return if the key is not found.
         * @retval The String object read from NVS, or the default value if the key doesn't exist.
         */
        String readString(const char *key, String def = String(""));

        /**
         * @brief Reads a binary blob of data from the specified key.
         * @param key The key to read the data from.
         * @param buf A pointer to the buffer to store the data.
         * @param max_size The maximum size of the buffer.
         * @retval The number of bytes read.
         */
        size_t read(const char *key, uint8_t *buf, size_t max_size);

        /**
         * @brief Checks if a key exists in the current namespace.
         * @param key The key to check for existence.
         * @retval true if the key exists, false otherwise.
         */
        bool exists(const char *key);

        /**
         * @brief Returns the data type of the value associated with a key.
         * @param key The key to get the type of.
         * @retval The PreferenceType of the key, or `PT_INVALID` if the key doesn't exist.
         */
        PreferenceType getType(const char *key);

        /**
         * @brief Returns the size of the data associated with a key.
         * @param key The key to get the size of.
         * @retval The size of the data in bytes, or 0 if the key is not found.
         */
        size_t getSize(const char *key);

    private:
        /// @brief Pointer to preferences instance.
        Preferences *pref;

        /// @brief A mutex for making it thread-safe.
        SemaphoreHandle_t mtx;

        /// @brief Flag for initiating the class only once.
        volatile bool is_init;
    };
}

#endif