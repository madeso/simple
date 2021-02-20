#include "engine/stringcounter.h"

namespace SimpleEngine
{
    void StringCounter::add(const std::string& s)
    {
        const int c = countsOf(s);
        counts[s] = c + 1;
    }

    int StringCounter::countsOf(const std::string& s) const
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
