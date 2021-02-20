#pragma once

#include <stdexcept>
#include <string>

namespace SimpleEngine
{
    struct UserException : public std::runtime_error
    {
        UserException(const std::string& message);
    };
}
