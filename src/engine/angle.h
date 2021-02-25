#pragma once

#include <string>

namespace SimpleEngine
{
    struct Angle
    {
        std::string ToString() const;

        static Angle Zero();

        float inRadians() const;

        float inDegrees() const;

        static Angle FromRadians(float r);

        static Angle FromDegrees(float deg);

        static Angle FromPercentOf360(float percent);

        Angle operator-() const;

        Angle Negative() const;

        float Sin() const;
        float Cos() const;
        float Tan();

        static Angle Asin(float v);

        static Angle Acos(float v);

        static Angle Atan(float v);

    private:
        float rad;

        Angle(float r);
    };

    Angle operator+(Angle l, Angle r);
    Angle operator-(Angle l, Angle r);
    Angle operator*(Angle l, float r);
}
