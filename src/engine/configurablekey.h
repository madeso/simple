#pragma once

#include <SDL.h>

#include <string>

namespace simple
{
    struct ConfigurableKey
    {
        enum class Type
        {
            Key
        };

        Type type;
        SDL_Keycode key;

        explicit ConfigurableKey(SDL_Keycode k);

        std::string ToString() const;

        bool operator==(const ConfigurableKey& rhs) const;
    };
}
