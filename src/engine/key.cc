#include "engine/key.h"

namespace simple
{
    void Button::Run(const ConfigurableKey& b, bool s, const std::vector<std::shared_ptr<Button>>& keys)
    {
        for (auto k : keys) k->run(b, s);
    }

    vec3 Button::Combine(std::shared_ptr<Button> x, std::shared_ptr<Button> y, std::shared_ptr<Button> z)
    {
        return vec3(x->Value(), y->Value(), z->Value());
    }

    Hold::Hold(const ConfigurableKey& k)
        : b(k)
    {
    }

    void Hold::run(const ConfigurableKey& k, bool s)
    {
        if (b == k)
        {
            status = s;
        }
    }

    bool Hold::IsDown()
    {
        return status;
    }

    int Hold::Value()
    {
        return IsDown() ? 1 : 0;
    }

    PlusMinus::PlusMinus(std::shared_ptr<Button> p, std::shared_ptr<Button> m)
        : plus(p)
        , minus(m)
    {
    }

    void PlusMinus::run(const ConfigurableKey& b, bool s)
    {
        plus->run(b, s);
        minus->run(b, s);
    }

    int PlusMinus::Value()
    {
        return plus->Value() - minus->Value();
    }
}
