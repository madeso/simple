#include "engine/angle.h"

#include <cmath>

#include "engine/math1.h"
#include "fmt/core.h"

namespace
{
    float rad2deg(float rad)
    {
        return (float)(180 / SimpleEngine::math1::pi) * rad;
    }

    float deg2rad(float deg)
    {
        return (float)SimpleEngine::math1::pi / 180 * deg;
    }
}

namespace SimpleEngine
{
    std::string Angle::ToString() const
    {
        return fmt::format("{}", inDegrees());
    }

    Angle Angle::Zero()
    {
        return Angle::FromRadians(0);
    }

    float Angle::inRadians() const
    {
        return rad;
    }

    float Angle::inDegrees() const
    {
        return rad2deg(rad);
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
        return Angle::FromRadians((float)(percent * math1::pi * 2));
    }

    Angle Angle::operator-() const
    {
        return Negative();
    }

    Angle Angle::Negative() const
    {
        return Angle{-rad};
    }

    float Angle::Sin() const
    {
        return (float)std::sin(inRadians());
    }

    float Angle::Cos() const
    {
        return (float)std::cos(inRadians());
    }

    float Angle::Tan()
    {
        return (float)std::tan(inRadians());
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
        : rad(r)
    {
    }

    Angle operator+(Angle l, Angle r)
    {
        return Angle::FromRadians(l.inRadians() + r.inRadians());
    }

    Angle operator-(Angle l, Angle r)
    {
        return Angle::FromRadians(l.inRadians() - r.inRadians());
    }

    Angle operator*(Angle l, float r)
    {
        return Angle::FromRadians(l.inRadians() * r);
    }
}
