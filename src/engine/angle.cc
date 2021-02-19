#include "engine/angle.h"

#include <cmath>

#include "fmt/core.h"

namespace
{
    constexpr float pi = M_PI;

    float rad2deg(float rad)
    {
        return (float)(180 / pi) * rad;
    }

    float deg2rad(float deg)
    {
        return (float)pi / 180 * deg;
    }
}

namespace SimpleEngine
{
    std::string angle::ToString() const
    {
        return fmt::format("{}", inDegrees());
    }

    angle angle::Zero()
    {
        return angle::FromRadians(0);
    }

    float angle::inRadians() const
    {
        return rad;
    }

    float angle::inDegrees() const
    {
        return rad2deg(rad);
    }

    angle angle::FromRadians(float r)
    {
        return angle{r};
    }

    angle angle::FromDegrees(float deg)
    {
        return angle{deg2rad(deg)};
    }

    angle angle::FromPercentOf360(float percent)
    {
        return angle::FromRadians((float)(percent * pi * 2));
    }

    angle angle::operator-() const
    {
        return Negative();
    }

    angle angle::Negative() const
    {
        return angle{-rad};
    }

    float angle::Sin() const
    {
        return (float)std::sin(inRadians());
    }

    float angle::Cos() const
    {
        return (float)std::cos(inRadians());
    }

    float angle::Tan()
    {
        return (float)std::tan(inRadians());
    }

    angle angle::Asin(float v)
    {
        return angle::FromRadians((float)std::asin(v));
    }

    angle angle::Acos(float v)
    {
        return angle::FromRadians((float)std::acos(v));
    }

    angle angle::Atan(float v)
    {
        return angle::FromRadians((float)std::atan(v));
    }

    angle::angle(float r)
        : rad(r)
    {
    }

    angle operator+(angle l, angle r)
    {
        return angle::FromRadians(l.inRadians() + r.inRadians());
    }

    angle operator-(angle l, angle r)
    {
        return angle::FromRadians(l.inRadians() - r.inRadians());
    }

    angle operator*(angle l, float r)
    {
        return angle::FromRadians(l.inRadians() * r);
    }
}
