#include "engine/strings.h"

#include <algorithm>
#include <cctype>
#include <locale>
#include <sstream>

// https://stackoverflow.com/a/217605
namespace detail
{
    // trim from start (in place)
    void ltrim(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }));
    }

    // trim from end (in place)
    void rtrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }).base(),
                s.end());
    }

    // trim from both ends (in place)
    void trim(std::string& s)
    {
        ltrim(s);
        rtrim(s);
    }

    // trim from start (copying)
    std::string ltrim_copy(std::string s)
    {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    std::string rtrim_copy(std::string s)
    {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    std::string trim_copy(std::string s)
    {
        trim(s);
        return s;
    }
}

namespace simple
{
    std::string ToLower(const std::string& s)
    {
        std::string data = s;
        std::transform(data.begin(), data.end(), data.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return data;
    }

    std::string Nullstring(const std::string& str, const std::string& empty)
    {
        return str.empty()==false ? str : empty;
    }

    std::string Trim(const std::string& str)
    {
        return detail::trim_copy(str);
    }

    bool StartsWith(const std::string& fullString, const std::string& start)
    {
        return fullString.rfind(start, 0) == 0;
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
