#pragma once

#include <string>

namespace unqlitepp
{

struct Error
{
    int code;
    std::string log;
};

} // namespace unqlitepp
