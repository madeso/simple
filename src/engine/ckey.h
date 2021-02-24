#pragma once

#include <SDL.h>

#include <string>

namespace SimpleEngine
{
    struct Ckey
    {
        enum class Type
        {
            Key
        };

        Type type;
        SDL_Keycode key;

        explicit Ckey(SDL_Keycode k);

        std::string ToString() const;

        bool operator==(const Ckey& rhs) const;
    };
}
