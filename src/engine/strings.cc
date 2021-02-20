#include "engine/strings.h"

namespace SimpleEngine
{
    namespace Strings
    {
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
}
