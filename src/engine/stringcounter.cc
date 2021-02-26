#include "engine/stringcounter.h"

namespace simple
{
    void StringCounter::Add(const std::string& s)
    {
        const int c = GetCountsOf(s);
        counts[s] = c + 1;
    }

    int StringCounter::GetCountsOf(const std::string& s) const
    {
        auto found = counts.find(s);
        if (found == counts.end())
        {
            return 0;
        }
        else
        {
            return found->second;
        }
    }
}
