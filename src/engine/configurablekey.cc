#include "engine/configurablekey.h"

#include <cassert>

#include "fmt/core.h"

namespace simple
{
    ConfigurableKey::ConfigurableKey(SDL_Keycode k)
        : type(Type::Key)
        , key(k)
    {
        // this.mouse = MouseButtons.None;
    }

    std::string ConfigurableKey::ToString() const
    {
        assert(type == Type::Key);
        return fmt::format("key: {0}", SDL_GetKeyName(key));
    }

    bool ConfigurableKey::operator==(const ConfigurableKey& rhs) const
    {
        return type == rhs.type && key == rhs.key;
    }
}
