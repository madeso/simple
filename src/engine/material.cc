﻿#include "engine/material.h"

#include "engine/opengl.h"

namespace simple
{
    Material::Material()
        : color(vec3(1, 1, 1))
    {
    }

    Material::Material(const vec3& c, std::shared_ptr<TextureMedia> t)
        : color(c)
        , texture(t)
    {
    }

    void Material::Apply()
    {
        glColor3f(color.x, color.y, color.z);
        texture->Bind();
    }
}
