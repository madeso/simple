#pragma once

#include <map>
#include <string>

namespace simple
{
    struct StringCounter
    {
        std::map<std::string, int> counts;

        void Add(const std::string& s);

        int GetCountsOf(const std::string& s) const;
    };
}
