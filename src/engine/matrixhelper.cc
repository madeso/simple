#include "engine/matrixhelper.h"

#include "engine/mat33.h"
#include "engine/quat.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    MatrixHelper::MatrixHelper(const SimpleEngine::mat44& m)
        : mat(m)
    {
    }

    MatrixHelper& MatrixHelper::mult(const SimpleEngine::mat44& m)
    {
        mat = mat * m;
        return *this;
    }

    MatrixHelper& MatrixHelper::Rotate(const AxisAngle& aa)
    {
        return mult(mat44::FromAxisAngle(aa));
    }

    MatrixHelper& MatrixHelper::Rotate(const quat& q)
    {
        return mult(q.Conjugate().GetMatrix33().mat44());
    }

    MatrixHelper& MatrixHelper::Translate(const vec3& t)
    {
        return mult(mat44::TranslationFor(t));
    }

    vec3 MatrixHelper::transform(const vec3& v) const
    {
        return mat * v;
    }

    mat33 MatrixHelper::mat33() const
    {
        return mat.mat33();
    }

    mat44 MatrixHelper::mat44() const
    {
        return mat;
    }
}
