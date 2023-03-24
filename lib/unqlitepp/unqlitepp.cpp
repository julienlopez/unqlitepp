#include "unqlitepp.hpp"

#include <unqlite.h>

namespace unqlitepp
{

Database::~Database()
{
    if(m_ptr)
        unqlite_close(m_ptr);
}

Database::Database(Database&& other)
    : m_ptr{other.m_ptr}
{
    other.m_ptr = nullptr;
}

Database& Database::operator=(Database&& other)
{
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;
    return *this;
}

namespace
{

    constexpr int convertOpenMode(const OpenMode mode)
    {
        switch(mode)
        {
        case OpenMode::Create:
            return UNQLITE_OPEN_CREATE;
        case OpenMode::ReadWrite:
            return UNQLITE_OPEN_READWRITE;
        case OpenMode::ReadOnly:
            return UNQLITE_OPEN_READONLY;
        case OpenMode::Mmap:
            return UNQLITE_OPEN_MMAP | UNQLITE_OPEN_READONLY;
        case OpenMode::Temp:
            return UNQLITE_OPEN_TEMP_DB;
        }
        return -1;
    }

    std::string errorLog(const Database& db)
    {
        const char* buffer = nullptr;
        int length;
        unqlite_config(db.m_ptr, UNQLITE_CONFIG_ERR_LOG, &buffer, &length);
        return buffer;
    }

    tl::unexpected<Error> wrapError(const Database& db, const int return_code)
    {
        auto log = errorLog(db);
        if(return_code != UNQLITE_BUSY && return_code != UNQLITE_NOTIMPLEMENTED) unqlite_rollback(db.m_ptr);
        return tl::make_unexpected(Error{return_code, std::move(log)});
    }

    tl::expected<unqlite_int64, Error> getDataSize(Database& database, const std::string& key)
    {
        unqlite_int64 data_size;
        const auto res = unqlite_kv_fetch(database.m_ptr, key.c_str(), -1, nullptr, &data_size);
        if(res == UNQLITE_OK)
            return data_size;
        else
            return wrapError(database, res);
    }

    auto extractData(Database& database, const std::string& key)
    {
        return [&database, &key](unqlite_int64 data_size) -> tl::expected<std::string, Error>
        {
            std::string buffer(data_size, ' ');
            const auto res = unqlite_kv_fetch(database.m_ptr, key.c_str(), -1, buffer.data(), &data_size);
            if(res == UNQLITE_OK)
                return buffer;
            else
                return wrapError(database, res);
        };
    }

} // namespace

tl::expected<Database, Error> openLocalFile(const std::filesystem::path& file, const OpenMode mode)
{
    Database db;
    const auto res = unqlite_open(&db.m_ptr, file.string().c_str(), convertOpenMode(mode));
    if(res == UNQLITE_OK)
        return std::move(db);
    else
        return wrapError(db, res);
}

tl::expected<Database, Error> openInMemory(const OpenMode mode)
{
    Database db;
    const auto res = unqlite_open(&db.m_ptr, nullptr, convertOpenMode(mode) | UNQLITE_OPEN_IN_MEMORY);
    if(res == UNQLITE_OK)
        return db;
    else
        return wrapError(db, res);
}

tl::expected<void, Error> store(Database& database, const std::string& key, const std::string& data)
{
    const auto res = unqlite_kv_store(database.m_ptr, key.c_str(), -1, data.data(), data.size());
    if(res != UNQLITE_OK)
        return wrapError(database, res);
    else
        return {};
}

tl::expected<void, Error> append(Database& database, const std::string& key, const std::string& data)
{
    const auto res = unqlite_kv_append(database.m_ptr, key.c_str(), -1, data.data(), data.size());
    if(res != UNQLITE_OK)
        return wrapError(database, res);
    else
        return {};
}

tl::expected<std::string, Error> fetch(Database& database, const std::string& key)
{
    return getDataSize(database, key).and_then(extractData(database, key));
}

tl::expected<void, Error> erase(Database& database, const std::string& key)
{
    const auto res = unqlite_kv_delete(database.m_ptr, key.c_str(), -1);
    if(res != UNQLITE_OK)
        return wrapError(database, res);
    else
        return {};
}

} // namespace unqlitepp
