#include "engine/arcball.h"

#include <cmath>

#include "engine/axisangle.h"
#include "engine/quat.h"

namespace simple
{
    ArcBall::ArcBall(const vec2& c, float r)
        : center(c)
        , radius(r)
    {
    }

    vec3 Transform(const ArcBall& b, const vec2& v)
    {
        const auto x = (v.x - b.center.x) / b.radius;
        const auto y = (v.y - b.center.y) / b.radius;
        const auto rsq = x * x + y * y;
        const auto z = (rsq < 1) ? std::sqrt(1 - rsq) : 0.0f;
        return vec3(x, y, z).GetNormalized();
    }

    quat ArcBall::GetRotation(const vec2& from, const vec2& to) const
    {
        const auto f = Transform(*this, from);
        const auto t = Transform(*this, to);
        const auto axis = vec3::cross(f, t).GetNormalized();
        const auto angle = vec3::AngleBetween(f, t);
        return quat(AxisAngle::RightHandAround(axis, angle));
    }
}
