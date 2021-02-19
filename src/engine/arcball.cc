#include "engine/arcball.h"

#include <cmath>

#include "engine/axisangle.h"
#include "engine/quat.h"

namespace SimpleEngine
{
    ArcBall::ArcBall(const vec2& c, float r)
        : center(c)
        , radius(r)
    {
    }

    vec3 ArcBall::transform(const vec2& v) const
    {
        float x = (v.x - center.x) / radius;
        float y = (v.y - center.y) / radius;
        float rsq = x * x + y * y;
        float z = (rsq < 1) ? (float)(std::sqrt(1 - rsq)) : 0.0f;
        return vec3(x, y, z).Normalized();
    }

    quat ArcBall::rotation(const vec2& from, const vec2& to) const
    {
        vec3 f = transform(from);
        vec3 t = transform(to);
        vec3 axis = vec3::cross(f, t).Normalized();
        angle angle = vec3::AngleBetween(f, t);
        return quat(AxisAngle::RightHandAround(axis, angle));
    }
}
