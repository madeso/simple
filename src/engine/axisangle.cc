#include "engine/axisangle.h"

#include "fmt/core.h"

namespace simple
{
    std::string AxisAngle::ToString() const
    {
        return fmt::format("({} {})", axis.ToString(), angle.ToString());
    }

    AxisAngle::AxisAngle(const vec3& ax, const simple::Angle& an)
        : axis(ax)
        , angle(an)
    {
    }

    AxisAngle AxisAngle::RightHandAround(const vec3& axis, const simple::Angle& angle)
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
