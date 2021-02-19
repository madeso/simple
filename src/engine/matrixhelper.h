#pragma once

#include "engine/mat44.h"

namespace SimpleEngine
{
    struct AxisAngle;
    struct vec3;
    struct quat;
    struct mat33;

    struct MatrixHelper
    {
        mat44 mat;

        MatrixHelper(const mat44& m);

        MatrixHelper& mult(const mat44& m);

        MatrixHelper& Rotate(const AxisAngle& aa);
        MatrixHelper& Rotate(const quat& q);
        MatrixHelper& Translate(const vec3& t);

        vec3 transform(const vec3& v) const;
        mat33 mat33() const;
        mat44 mat44() const;
    };
}
