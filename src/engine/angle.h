#pragma once

#include <string>

namespace SimpleEngine
{
    struct angle
    {
        std::string ToString() const;

        static angle Zero();

        float inRadians() const;

        float inDegrees() const;

        static angle FromRadians(float r);

        static angle FromDegrees(float deg);

        static angle FromPercentOf360(float percent);

        angle operator-() const;

        angle Negative() const;

        float Sin() const;
        float Cos() const;
        float Tan();

        static angle Asin(float v);

        static angle Acos(float v);

        static angle Atan(float v);

    private:
        float rad;

        angle(float r);
    };

    angle operator+(angle l, angle r);
    angle operator-(angle l, angle r);
    angle operator*(angle l, float r);
}
