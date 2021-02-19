#pragma once

#include "engine/vec2.h"

namespace SimpleEngine
{
    struct quat;
    struct vec3;

    struct ArcBall
    {
        ArcBall(const vec2& c, float r);

        vec3 transform(const vec2& v) const;

        quat rotation(const vec2& from, const vec2& to) const;

        vec2 center;
        float radius;
    };
}
