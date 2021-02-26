#pragma once

#include <functional>
#include <vector>

namespace simple
{
    template <typename T>
    struct Pool
    {
        using TCreator = std::function<T()>;
        TCreator creator;
        std::vector<T> pool;
        Pool(TCreator c)
            : creator(std::move(c))
        {
        }

        T Create()
        {
            if (pool.size() == 0)
                return creator();
            else
            {
                auto r = *pool.rbegin();
                pool.pop_back();
                return r;
            }
        }

        void Destroy(T t)
        {
            pool.emplace_back(t);
        }
    };
}
