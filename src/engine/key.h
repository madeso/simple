#pragma once

#include <memory>
#include <vector>

#include "engine/ckey.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    struct Key
    {
        virtual void run(const Ckey& b, bool s) = 0;
        virtual int Value() = 0;

        static void Run(const Ckey& b, bool s, const std::vector<std::shared_ptr<Key>>& keys);

        static vec3 Combine(std::shared_ptr<Key> x, std::shared_ptr<Key> y, std::shared_ptr<Key> z);
    };

    struct Hold : public Key
    {
        bool status = false;
        Ckey b;

        Hold(const Ckey& k);

        void run(const Ckey& k, bool s) override;

        bool IsDown();

        int Value() override;
    };

    struct PlusMinus : public Key
    {
        std::shared_ptr<Key> plus;
        std::shared_ptr<Key> minus;

        PlusMinus(std::shared_ptr<Key> p, std::shared_ptr<Key> m);

        void run(const Ckey& b, bool s) override;

        int Value() override;
    };
}
