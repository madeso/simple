#pragma once

#include "catchy/falsestring.h"

namespace SimpleEngine
{
    struct quat;
    struct vec3;
    struct AxisAngle;
    struct mat33;
    struct mat44;

    catchy::FalseString AreEqual(float a, float b);
    catchy::FalseString AreEqual(const quat& a, const quat& b);
    catchy::FalseString AreEqual(const vec3& a, const vec3& b);
    catchy::FalseString AreEqual(const AxisAngle& a, const AxisAngle& b);
    catchy::FalseString AreEqual(const mat33& a, const mat33& b);
    catchy::FalseString AreEqual(const mat44& a, const mat44& b);
}
