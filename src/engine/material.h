#pragma once

#include "engine/texture.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    struct Material
    {
        Material();

        Material(const vec3& c, std::shared_ptr<Texture> t);

        void apply();

        vec3 color;
        std::shared_ptr<Texture> tex;
    };
}
