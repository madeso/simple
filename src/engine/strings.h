#pragma once

#include <string>

namespace SimpleEngine
{
    std::string ToLower(const std::string& s);
    std::string Nullstring(const std::string& nullorvalue, const std::string& valid);

    std::string FirstChars(const std::string& s, int count, const std::string& extra);
    std::string FirstChars(const std::string& s, int count);
}
