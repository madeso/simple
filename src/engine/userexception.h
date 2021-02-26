#pragma once

#include <stdexcept>
#include <string>

namespace simple
{
    struct UserException : public std::runtime_error
    {
        UserException(const std::string& message);
    };
}
