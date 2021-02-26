#pragma once

#include <string>
#include <vector>

namespace simple
{
    std::string ToLower(const std::string& s);
    std::string Nullstring(const std::string& nullorvalue, const std::string& valid);

    std::string Trim(const std::string& str);
    bool EndsWith(const std::string& str, const std::string& s);
    bool StartsWith(const std::string& str, const std::string& s);

    std::vector<std::string> Split(const std::string& str, char delim);
    std::vector<std::string> Split(const std::string& str);

    std::string FirstChars(const std::string& s, int count, const std::string& extra);
    std::string FirstChars(const std::string& s, int count);
}
