#pragma once

#include <string>

#include "engine/angle.h"
#include "engine/vec3.h"

namespace simple
{
    struct AxisAngle
    {
        std::string ToString() const;

        AxisAngle(const vec3& axis, const Angle& angle);

        static AxisAngle RightHandAround(const vec3& axis, const Angle& angle);

        AxisAngle Negative() const;

        vec3 axis;
        simple::Angle angle;
    };

    AxisAngle operator-(const AxisAngle& me);
}
