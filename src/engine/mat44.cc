#include "engine/mat44.h"

#include "engine/axisangle.h"
#include "engine/mat33.h"
#include "engine/matrixhelper.h"
#include "engine/quat.h"
#include "engine/vec3.h"
#include "fmt/core.h"

namespace simple
{
    namespace
    {
        float multsum(const mat44& a, const mat44& b, int row, int col)
        {
            return a(row, 0) * b(0, col) + a(row, 1) * b(1, col) + a(row, 2) * b(2, col) + a(row, 3) * b(3, col);
        }

        float multsum(const mat44& a, const vec3& b, int row)
        {
            return a(row, 0) * b(0) + a(row, 1) * b(1) + a(row, 2) * b(2) + a(row, 3) * 1;
        }
    }

    std::string mat44::ToString() const
    {
        const auto& m = column_major;
        return fmt::format(
            "\n| {0} {4} {8} {12} |"
            "\n| {1} {5} {9} {13} |"
            "\n| {2} {5} {10} {14} |"
            "\n| {3} {7} {11} {15} |",
            m[0], m[1], m[2], m[3], m[4], m[5], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
    }

    float* mat44::AsColumnMajor()
    {
        return &column_major[0];
    }

    float mat44::operator()(int row, int column) const
    {
        return column_major[row + column * kSize];
    }
    float& mat44::operator()(int row, int column)
    {
        return column_major[row + column * kSize];
    }

    mat44::mat44()
        : column_major(FA{
              1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0,
              0, 0, 0, 1})
    {
    }

    mat44::mat44(mat44::FA d)
        : column_major(d)
    {
    }

    mat44 mat44::FromColumnMajor(FA data)
    {
        return mat44(data);
    }

    mat44 mat44::FromRowMajor(FA data)
    {
        return mat44({data[0], data[4], data[8], data[12],
                      data[1], data[5], data[9], data[13],
                      data[2], data[6], data[10], data[14],
                      data[3], data[7], data[11], data[15]});
    }

    mat44 mat44::FromTranslation(const vec3& v)
    {
        return FromRowMajor(FA{
            1, 0, 0, v.x,
            0, 1, 0, v.y,
            0, 0, 1, v.z,
            0, 0, 0, 1});
    }

    simple::mat33 mat44::AsMat33() const
    {
        const auto& self = *this;
        return simple::mat33::FromRowMajor({self(0, 0), self(0, 1), self(0, 2),
                                                  self(1, 0), self(1, 1), self(1, 2),
                                                  self(2, 0), self(2, 1), self(2, 2)});
    }

    vec3 mat44::GetLocation() const
    {
        const auto& self = *this;
        return vec3(self(0, 3), self(1, 3), self(2, 3));
    }

    MatrixHelper mat44::Help() const
    {
        return MatrixHelper(*this);
    }

    mat44 mat44::FromAxisAngle(const AxisAngle& aa)
    {
        float c = aa.angle.Cos();
        float s = aa.angle.Sin();
        float x = aa.axis.x;
        float y = aa.axis.y;
        float z = aa.axis.z;

        return mat44(FA{
            x * x * (1 - c) + c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0,
            y * x * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - x * s, 0,
            x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z * (1 - c) + c, 0,
            0, 0, 0, 1});
    }

    mat44 mat44::Identity()
    {
        return mat44(FA{
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1});
    }

    mat44 operator*(const mat44& a, const mat44& b)
    {
        return mat44::FromRowMajor(mat44::FA{
            multsum(a, b, 0, 0),
            multsum(a, b, 0, 1),
            multsum(a, b, 0, 2),
            multsum(a, b, 0, 3),
            multsum(a, b, 1, 0),
            multsum(a, b, 1, 1),
            multsum(a, b, 1, 2),
            multsum(a, b, 1, 3),
            multsum(a, b, 2, 0),
            multsum(a, b, 2, 1),
            multsum(a, b, 2, 2),
            multsum(a, b, 2, 3),
            multsum(a, b, 3, 0),
            multsum(a, b, 3, 1),
            multsum(a, b, 3, 2),
            multsum(a, b, 3, 3),
        });
    }

    vec3 operator*(const mat44& m, const vec3& v)
    {
        return vec3(multsum(m, v, 0), multsum(m, v, 1), multsum(m, v, 2));
    }
}
