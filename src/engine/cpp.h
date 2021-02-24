#include <algorithm>
#include <string>

namespace SimpleEngine
{
    template <typename V, typename C>
    void Erase(V& v, C&& callback)
    {
        v.erase(std::remove_if(v.begin(), v.end(), callback), v.end());
    }

    std::string Nullstring(const std::string& str, const std::string& empty);
}
