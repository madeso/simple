#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace SimpleEngine
{
    struct StringSeperator
    {
        StringSeperator(const std::string& aSeperator, const std::string& aFinalSeperator, const std::string& empty);

        StringSeperator(const std::string& aSeperator, const std::string& aFinalSeperator);

        StringSeperator(const std::string& aSeperator);

        std::string ToString(const std::vector<std::string>& mstrings) const;

        std::string mSeperator;
        std::string mFinalSeperator;
        std::string mEmpty;
    };
}
