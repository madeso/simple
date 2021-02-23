#include "engine/ckey.h"

#include <cassert>

#include "fmt/core.h"

namespace SimpleEngine
{
    Ckey::Ckey(SDL_Keycode k)
        : type(Type::Key)
        , key(k)
    {
        // this.mouse = MouseButtons.None;
    }

    std::string Ckey::ToString() const
    {
        assert(type == Type::Key);
        return fmt::format("key: {0}", SDL_GetKeyName(key));
    }

    bool Ckey::operator==(const Ckey& rhs) const
    {
        return type == rhs.type && key == rhs.key;
    }
}
