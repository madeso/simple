#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace simple
{
    struct StringSeperator
    {
        StringSeperator(const std::string& s, const std::string& fs, const std::string& e);

        StringSeperator(const std::string& s, const std::string& fs);

        StringSeperator(const std::string& s);

        std::string ToString(const std::vector<std::string>& strings) const;

        std::string seperator;
        std::string final_seperator;
        std::string empty;
    };
}
