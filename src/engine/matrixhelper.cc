#include "engine/matrixhelper.h"

#include "engine/mat33.h"
#include "engine/quat.h"
#include "engine/vec3.h"

namespace simple
{
    MatrixHelper::MatrixHelper(const simple::mat44& m)
        : matrix(m)
    {
    }

    MatrixHelper& MatrixHelper::Multiply(const simple::mat44& m)
    {
        matrix = matrix * m;
        return *this;
    }

    MatrixHelper& MatrixHelper::Rotate(const AxisAngle& aa)
    {
        return Multiply(mat44::FromAxisAngle(aa));
    }

    MatrixHelper& MatrixHelper::Rotate(const quat& q)
    {
        return Multiply(q.Conjugate().GetMatrix33().AsMat44());
    }

    MatrixHelper& MatrixHelper::Translate(const vec3& t)
    {
        return Multiply(mat44::FromTranslation(t));
    }

    vec3 MatrixHelper::GetTransform(const vec3& v) const
    {
        return matrix * v;
    }

    mat33 MatrixHelper::AsMat33() const
    {
        return matrix.AsMat33();
    }

    mat44 MatrixHelper::AsMat44() const
    {
        return matrix;
    }
}
