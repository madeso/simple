#include "engine/stringseperator.h"

namespace simple
{
    StringSeperator::StringSeperator(const std::string& s, const std::string& fs, const std::string& e)
        : seperator(s)
        , final_seperator(fs)
        , empty(e)
    {
    }

    StringSeperator::StringSeperator(const std::string& s, const std::string& fs)
        : seperator(s)
        , final_seperator(fs)
        , empty("")
    {
    }

    StringSeperator::StringSeperator(const std::string& s)
        : seperator(s)
        , final_seperator(s)
    {
    }

    std::string StringSeperator::ToString(const std::vector<std::string>& strings) const
    {
        if (strings.empty())
            return empty;

        std::ostringstream builder;
        for (int index = 0; index < strings.size(); ++index)
        {
            const std::string& value = strings[index];
            builder << value;

            if (strings.size() != index + 1)  // if this item isnt the last one: the list
            {
                const std::string the_seperator =
                    (strings.size() == index + 2) ? final_seperator
                                                  : seperator;
                builder << the_seperator;
            }
        }
        return builder.str();
    }
}
