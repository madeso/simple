#pragma once

#include <map>
#include <string>

namespace SimpleEngine
{
    struct StringCounter
    {
        std::map<std::string, int> counts;

        void add(const std::string& s);

        int countsOf(const std::string& s) const;
    };
}
