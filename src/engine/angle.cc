#include "engine/angle.h"

#include <cmath>

#include "engine/math.h"
#include "fmt/core.h"

namespace
{
    float rad2deg(float rad)
    {
        return (float)(180 / simple::math::pi) * rad;
    }

    float deg2rad(float deg)
    {
        return (float)simple::math::pi / 180 * deg;
    }
}

namespace simple
{
    std::string Angle::ToString() const
    {
        return fmt::format("{}", InDegrees());
    }

    Angle Angle::Zero()
    {
        return Angle::FromRadians(0);
    }

    float Angle::InRadians() const
    {
        return radians;
    }

    float Angle::InDegrees() const
    {
        return rad2deg(radians);
    }

    Angle Angle::FromRadians(float r)
    {
        return Angle{r};
    }

    Angle Angle::FromDegrees(float deg)
    {
        return Angle{deg2rad(deg)};
    }

    Angle Angle::FromPercentOf360(float percent)
    {
        return Angle::FromRadians((float)(percent * math::pi * 2));
    }

    Angle Angle::operator-() const
    {
        return Negative();
    }

    Angle Angle::Negative() const
    {
        return Angle{-radians};
    }

    float Angle::Sin() const
    {
        return (float)std::sin(InRadians());
    }

    float Angle::Cos() const
    {
        return (float)std::cos(InRadians());
    }

    float Angle::Tan()
    {
        return (float)std::tan(InRadians());
    }

    Angle Angle::Asin(float v)
    {
        return Angle::FromRadians((float)std::asin(v));
    }

    Angle Angle::Acos(float v)
    {
        return Angle::FromRadians((float)std::acos(v));
    }

    Angle Angle::Atan(float v)
    {
        return Angle::FromRadians((float)std::atan(v));
    }

    Angle::Angle(float r)
        : radians(r)
    {
    }

    Angle operator+(Angle l, Angle r)
    {
        return Angle::FromRadians(l.InRadians() + r.InRadians());
    }

    Angle operator-(Angle l, Angle r)
    {
        return Angle::FromRadians(l.InRadians() - r.InRadians());
    }

    Angle operator*(Angle l, float r)
    {
        return Angle::FromRadians(l.InRadians() * r);
    }
}
