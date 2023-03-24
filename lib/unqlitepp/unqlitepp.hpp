#pragma once

#include "error.hpp"

#include <filesystem>

#include <unqlite.h>

#include <tl/expected.hpp>

namespace unqlitepp
{

struct Database
{
    Database() = default;

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    Database(Database&&);
    Database& operator=(Database&&);

    ~Database();

    unqlite* m_ptr = nullptr;
};

enum class OpenMode
{
    Create,
    ReadWrite,
    ReadOnly,
    Mmap,
    Temp
};

tl::expected<Database, Error> openLocalFile(const std::filesystem::path& file, const OpenMode mode);

tl::expected<Database, Error> openInMemory(const OpenMode mode);

tl::expected<void, Error> store(Database& database, const std::string& key, const std::string& data);

tl::expected<void, Error> append(Database& database, const std::string& key, const std::string& data);

tl::expected<std::string, Error> fetch(Database& database, const std::string& key);

tl::expected<void, Error> erase(Database& database, const std::string& key);

} // namespace unqlitepp
