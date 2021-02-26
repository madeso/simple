#pragma once

#include "engine/vec2.h"

namespace simple
{
    struct quat;
    struct vec3;

    struct ArcBall
    {
        ArcBall(const vec2& c, float r);

        quat GetRotation(const vec2& from, const vec2& to) const;

        vec2 center;
        float radius;
    };
}
