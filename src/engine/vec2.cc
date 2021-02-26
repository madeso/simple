#include "engine/vec2.h"

#include "engine/math.h"
#include "fmt/core.h"

namespace simple
{
    vec2::vec2(float xx, float yy)
        : x(xx)
        , y(yy)
    {
    }

    vec2 vec2::Zero()
    {
        return vec2(0, 0);
    }

    std::string vec2::ToString() const
    {
        return fmt::format("({} {})", x, y);
    }

    vec2 vec2::GetScaled(float s) const
    {
        return vec2(x * s, y * s);
    }

    vec2 vec2::FromTo(const vec2& from, const vec2& to)
    {
        return to - from;
    }

    vec2 vec2::Curve(const vec2& target, const vec2& old, float smoothing)
    {
        return vec2(math::Curve(target.x, old.x, smoothing), math::Curve(target.y, old.y, smoothing));
    }

    vec2 operator+(const vec2& lhs, const vec2& rhs)
    {
        return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    vec2 operator-(const vec2& lhs, const vec2& rhs)
    {
        return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    vec2 operator*(const vec2& lhs, float rhs)
    {
        return lhs.GetScaled(rhs);
    }
}
