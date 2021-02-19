#pragma once

#include <array>
#include <string>

namespace SimpleEngine
{
    struct vec3;
    struct mat44;
    struct quat;

    // matrix order:3x3
    struct mat33
    {
        static constexpr int kSize = 3;
        using FA = std::array<float, kSize * kSize>;

        std::string ToString() const;

        /*
         *     | 0 3 6 |
         * M = | 1 4 7 |
         *     | 2 5 8 |
         */

        FA dataColumnMajor;

        float* DataArray();

        float operator()(int row, int column) const;
        float& operator()(int row, int column);

        mat33(FA data);

        static mat33 FromColumnMajor(FA data);

        static mat33 FromRowMajor(FA data);

        vec3 XAxis() const;

        vec3 YAxis() const;

        vec3 ZAxis() const;

        SimpleEngine::mat44 mat44() const;

        /*mat33 Transposed
        {
            get
            {
                return FromRowMajor(dataColumnMajor);
            }
        }*/

        static mat33 Identity();

        static mat33 Scale(const vec3& scale);

        SimpleEngine::quat quat() const;
    };
}
