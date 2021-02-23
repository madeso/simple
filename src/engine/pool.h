#pragma once

#include <functional>
#include <vector>

namespace SimpleEngine
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

        T get()
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

        void release(T t)
        {
            pool.emplace_back(t);
        }
    };
}
