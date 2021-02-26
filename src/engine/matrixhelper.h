#pragma once

#include "engine/mat44.h"

namespace simple
{
    struct AxisAngle;
    struct vec3;
    struct quat;
    struct mat33;

    struct MatrixHelper
    {
        mat44 matrix;

        MatrixHelper(const mat44& m);

        MatrixHelper& Multiply(const mat44& m);

        MatrixHelper& Rotate(const AxisAngle& aa);
        MatrixHelper& Rotate(const quat& q);
        MatrixHelper& Translate(const vec3& t);

        vec3 GetTransform(const vec3& v) const;
        mat33 AsMat33() const;
        mat44 AsMat44() const;
    };
}
