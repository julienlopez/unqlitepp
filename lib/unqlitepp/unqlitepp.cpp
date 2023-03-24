#include "unqlitepp.hpp"

#include <unqlite.h>

namespace unqlitepp
{

Database::~Database()
{
    unqlite_close(m_ptr);
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

    std::string errorLog(Database& db)
    {
        const char* buffer = nullptr;
        int length;
        unqlite_config(db.m_ptr, UNQLITE_CONFIG_ERR_LOG, &buffer, &length);
        if(length > 0 && buffer) puts(buffer);
        return buffer;
    }

    tl::unexpected<Error> wrapError(Database& db, const int return_code)
    {
        auto log = errorLog(db);
        if(return_code != UNQLITE_BUSY && return_code != UNQLITE_NOTIMPLEMENTED) unqlite_rollback(db.m_ptr);
        return tl::make_unexpected(Error{return_code, std::move(log)});
    }

} // namespace

tl::expected<Database, Error> openLocalFile(const std::filesystem::path& file, const OpenMode mode)
{
    Database db;
    const auto res = unqlite_open(&db.m_ptr, file.string().c_str(), convertOpenMode(mode));
    if(res == UNQLITE_OK)
        return db;
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

} // namespace unqlitepp
