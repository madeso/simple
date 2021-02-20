#include "engine/strings.h"

#include <algorithm>
#include <cctype>

namespace SimpleEngine
{
    std::string Lower(const std::string& s)
    {
        std::string data = s;
        std::transform(data.begin(), data.end(), data.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return data;
    }

    std::string Nullstring(const std::string& nullorvalue, const std::string& valid)
    {
        if (nullorvalue.empty())
        {
            return valid;
        }
        else
        {
            return nullorvalue;
        }
    }

    std::string FirstChars(const std::string& s, int count, const std::string& extra)
    {
        int l = s.length();
        if (s.length() + extra.length() > count)
            return s.substr(0, count - extra.length()) + extra;
        else
            return s;
    }

    std::string FirstChars(const std::string& s, int count)
    {
        return FirstChars(s, count, "...");
    }
}
