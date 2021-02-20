#pragma once

#include <string>

namespace SimpleEngine
{
    namespace Strings
    {
        std::string FirstChars(const std::string& s, int count, const std::string& extra);
        std::string FirstChars(const std::string& s, int count);
    }
}
