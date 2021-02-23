#include "engine/stringseperator.h"

namespace SimpleEngine
{
    StringSeperator::StringSeperator(const std::string& aSeperator, const std::string& aFinalSeperator, const std::string& empty)
        : mSeperator(aSeperator)
        , mFinalSeperator(aFinalSeperator)
        , mEmpty(empty)
    {
    }

    StringSeperator::StringSeperator(const std::string& aSeperator, const std::string& aFinalSeperator)
        : mSeperator(aSeperator)
        , mFinalSeperator(aFinalSeperator)
        , mEmpty("")
    {
    }

    StringSeperator::StringSeperator(const std::string& aSeperator)
        : mSeperator(aSeperator)
        , mFinalSeperator(aSeperator)
    {
    }

    std::string StringSeperator::ToString(const std::vector<std::string>& mstrings) const
    {
        if (mstrings.empty())
            return mEmpty;

        std::ostringstream builder;
        for (int index = 0; index < mstrings.size(); ++index)
        {
            const std::string& value = mstrings[index];
            builder << value;

            if (mstrings.size() != index + 1)  // if this item isnt the last one: the list
            {
                const std::string seperator =
                    (mstrings.size() == index + 2) ? mFinalSeperator
                                                   : mSeperator;
                builder << seperator;
            }
        }
        return builder.str();
    }
}
