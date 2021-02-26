#pragma once

#include <memory>
#include <vector>

#include "engine/configurablekey.h"
#include "engine/vec3.h"

namespace simple
{
    struct Button
    {
        virtual void run(const ConfigurableKey& b, bool s) = 0;
        virtual int Value() = 0;

        static void Run(const ConfigurableKey& b, bool s, const std::vector<std::shared_ptr<Button>>& keys);

        static vec3 Combine(std::shared_ptr<Button> x, std::shared_ptr<Button> y, std::shared_ptr<Button> z);
    };

    struct Hold : public Button
    {
        bool status = false;
        ConfigurableKey b;

        Hold(const ConfigurableKey& k);

        void run(const ConfigurableKey& k, bool s) override;

        bool IsDown();

        int Value() override;
    };

    struct PlusMinus : public Button
    {
        std::shared_ptr<Button> plus;
        std::shared_ptr<Button> minus;

        PlusMinus(std::shared_ptr<Button> p, std::shared_ptr<Button> m);

        void run(const ConfigurableKey& b, bool s) override;

        int Value() override;
    };
}
