#include "engine/key.h"

namespace SimpleEngine
{
    void Key::Run(const Ckey& b, bool s, const std::vector<std::shared_ptr<Key>>& keys)
    {
        for (auto k : keys) k->run(b, s);
    }

    vec3 Key::Combine(std::shared_ptr<Key> x, std::shared_ptr<Key> y, std::shared_ptr<Key> z)
    {
        return vec3(x->Value(), y->Value(), z->Value());
    }

    Hold::Hold(const Ckey& k)
        : b(k)
    {
    }

    void Hold::run(const Ckey& k, bool s)
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

    PlusMinus::PlusMinus(std::shared_ptr<Key> p, std::shared_ptr<Key> m)
        : plus(p)
        , minus(m)
    {
    }

    void PlusMinus::run(const Ckey& b, bool s)
    {
        plus->run(b, s);
        minus->run(b, s);
    }

    int PlusMinus::Value()
    {
        return plus->Value() - minus->Value();
    }
}
