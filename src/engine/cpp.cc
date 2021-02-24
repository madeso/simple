#include "engine/cpp.h"

namespace SimpleEngine
{
    std::string Nullstring(const std::string& str, const std::string& empty)
    {
        return str.empty() ? empty : str;
    }
}
