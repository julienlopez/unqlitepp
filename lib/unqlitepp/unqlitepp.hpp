#pragma once

#include "error.hpp"

#include <filesystem>

#include <unqlite.h>

#include <tl/expected.hpp>

namespace unqlitepp
{
struct Database
{
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
} // namespace unqlitepp
