#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace simple::fse
{
    template <typename T>
    struct Map
    {
        using TGenerator = std::function<T(std::string)>;

        std::map<std::string, T> already_created;
        TGenerator generator;

        Map(TGenerator&& g)
            : generator(std::move(g))
        {
        }

        T GetOrCreate(const std::string& name)
        {
            auto found = already_created.find(name);
            if (found != already_created.end())
            {
                return found->second;
            }

            T t = generator(name);
            Set(name, t);
            return t;
        }

        void Set(const std::string& name, T t)
        {
            already_created.emplace(name, t);
        }

        std::vector<T> GetCreatedValues()
        {
            std::vector<T> r;
            for (auto& kv : already_created)
            {
                r.emplace_back(kv.second);
            }
            return r;
        }
    };
}
