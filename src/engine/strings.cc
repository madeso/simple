#include "engine/strings.h"

#include <algorithm>
#include <cctype>
#include <sstream>

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

    bool EndsWith(const std::string& fullString, const std::string& ending)
    {
        if (fullString.length() >= ending.length())
        {
            return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
        }
        else
        {
            return false;
        }
    }

    std::vector<std::string> Split(const std::string& s, char delim)
    {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, delim))
        {
            result.push_back(item);
        }

        return result;
    }

    std::vector<std::string> Split(const std::string& s)
    {
        return Split(s, ' ');
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
