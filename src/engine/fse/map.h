#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace SimpleEngine::fse
{
    template <typename T>
    struct Map
    {
        using TGenerator = std::function<T(std::string)>;

        std::map<std::string, T> m;
        TGenerator generator;

        Map(TGenerator&& g)
            : generator(std::move(g))
        {
        }

        T get(const std::string& var)
        {
            auto found = m.find(var);
            if (found != m.end())
            {
                return found->second;
            }

            T t = generator(var);
            add(var, t);
            return t;
        }

        void add(const std::string& name, T t)
        {
            m.emplace(name, t);
        }

        std::vector<T> Data()
        {
            std::vector<T> r;
            for (auto& kv : m)
            {
                r.emplace_back(kv.second);
            }
            return r;
        }
    };
}
