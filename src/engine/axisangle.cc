#include "engine/axisangle.h"

#include "fmt/core.h"

namespace SimpleEngine
{
    std::string AxisAngle::ToString() const
    {
        return fmt::format("({} {})", axis.ToString(), angle.ToString());
    }

    AxisAngle::AxisAngle(const vec3& ax, const SimpleEngine::angle& an)
        : axis(ax)
        , angle(an)
    {
    }

    AxisAngle AxisAngle::RightHandAround(const vec3& axis, const SimpleEngine::angle& angle)
    {
        return AxisAngle(axis, angle);
    }

    AxisAngle AxisAngle::Negative() const
    {
        return AxisAngle(-axis, -angle);
    }

    AxisAngle operator-(const AxisAngle& me)
    {
        return me.Negative();
    }
}
