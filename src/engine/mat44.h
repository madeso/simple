#pragma once

#include <array>
#include <string>

namespace simple
{
    struct MatrixHelper;
    struct quat;
    struct vec3;
    struct mat33;
    struct AxisAngle;

    struct mat44
    {
        static constexpr int kSize = 4;
        using FA = std::array<float, kSize * kSize>;

        std::string ToString() const;

        /*
         *     | 0 4 8  12 |
         * M = | 1 5 9  13 |
         *     | 2 5 10 14 |
         *     | 3 7 11 15 |
         */

        FA column_major;
        float* AsColumnMajor();

        float operator()(int row, int column) const;
        float& operator()(int row, int column);

        mat44();
        mat44(FA d);

        static mat44 FromColumnMajor(FA data);

        static mat44 FromRowMajor(FA data);

        static mat44 FromTranslation(const vec3& v);

        simple::mat33 AsMat33() const;

        vec3 GetLocation() const;

        MatrixHelper Help() const;

        // override string ToString()
        // {
        //     return string.Format("({0} {1} {2} {3})", rowAsString(0), rowAsString(1), rowAsString(2), rowAsString(3));
        // }
        // string rowAsString(int row)
        // {
        //     return string.Format("({0} {1} {2} {3})", nice(this(row, 0)), nice(this(row, 1)), nice(this(row, 2)), nice(this(row, 3)));
        // }
        // float nice(float p)
        // {
        //     if (math1::IsZero(p)) return 0;
        //     else return p;
        // }

        static mat44 FromAxisAngle(const AxisAngle& aa);

        static mat44 Identity();
    };

    mat44 operator*(const mat44& a, const mat44& b);

    vec3 operator*(const mat44& m, const vec3& v);
}
