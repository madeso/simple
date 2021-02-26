#include <algorithm>
#include <string>

namespace simple
{
    template <typename V, typename C>
    void Erase(V& v, C&& callback)
    {
        v.erase(std::remove_if(v.begin(), v.end(), callback), v.end());
    }
}
