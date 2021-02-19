#include "engine/cpp.h"

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
}
