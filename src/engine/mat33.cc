#include "engine/mat33.h"

#include "engine/mat44.h"
#include "engine/math1.h"
#include "engine/quat.h"
#include "engine/vec3.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    std::string mat33::ToString() const
    {
        const auto& m = dataColumnMajor;

        return fmt::format(
            "\n| {0} {3} {6} |"
            "\n| {1} {4} {7} |"
            "\n| {2} {5} {8} |",
            m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8]);
    }

    float* mat33::DataArray()
    {
        return &dataColumnMajor[0];
    }

    float mat33::operator()(int row, int column) const
    {
        return dataColumnMajor[row + column * kSize];
    }
    float& mat33::operator()(int row, int column)
    {
        return dataColumnMajor[row + column * kSize];
    }

    mat33::mat33(FA data)
        : dataColumnMajor(data)
    {
    }

    mat33 mat33::FromColumnMajor(FA data)
    {
        return mat33(data);
    }

    mat33 mat33::FromRowMajor(FA data)
    {
        return mat33(FA{
            data[0], data[3], data[6],
            data[1], data[4], data[7],
            data[2], data[5], data[8]});
    }

    vec3 mat33::XAxis() const
    {
        const auto& self = *this;
        return vec3(self(0, 0), self(1, 0), self(2, 0));
    }

    vec3 mat33::YAxis() const
    {
        const auto& self = *this;
        return vec3(self(0, 1), self(1, 1), self(2, 1));
    }

    vec3 mat33::ZAxis() const
    {
        const auto& self = *this;
        return -vec3(self(0, 2), self(1, 2), self(2, 2));
    }

    mat44 mat33::mat44() const
    {
        const auto& self = *this;
        return mat44::FromRowMajor(mat44::FA{
            self(0, 0), self(0, 1), self(0, 2), 0,
            self(1, 0), self(1, 1), self(1, 2), 0,
            self(2, 0), self(2, 1), self(2, 2), 0,
            0, 0, 0, 1});
    }

    mat33 mat33::Identity()
    {
        return mat33(FA{
            1, 0, 0,
            0, 1, 0,
            0, 0, 1});
    }

    mat33 mat33::Scale(const vec3& scale)
    {
        return FromRowMajor(FA{
            scale.x, 0, 0,
            0, scale.y, 0,
            0, 0, scale.z});
    }

    quat mat33::quat() const
    {
        const auto& self = *this;

        // check the diagonal
        float tr = self(0, 0) + self(1, 1) + self(2, 2);
        if (tr > 0.0f)
        {
            float s = math1::Sqrt(tr + 1.0f);
            float t = 0.5f / s;

            return SimpleEngine::quat(
                s * 0.5f,
                vec3((self(1, 2) - self(2, 1)) * t,
                     (self(2, 0) - self(0, 2)) * t,
                     (self(0, 1) - self(1, 0)) * t));
        }
        else
        {
            std::array<int, 3> NXT = {1, 2, 0};
            SimpleEngine::quat q = SimpleEngine::quat::Identity();
            // diagonal is negative
            // get biggest diagonal element
            int i = 0;
            if (self(1, 1) > self(0, 0))
                i = 1;
            if (self(2, 2) > self(i, i))
                i = 2;
            //setup index sequence
            int j = NXT[i];
            int k = NXT[j];

            float s = math1::Sqrt((self(i, i) - (self(j, j) + self(k, k))) + 1.0f);

            q(i) = s * 0.5f;

            if (math1::IsZero(s) == false)
                s = 0.5f / s;

            q(j) = (self(i, j) + self(j, i)) * s;
            q(k) = (self(i, k) + self(k, i)) * s;
            q(3) = (self(j, k) - self(k, j)) * s;

            return q;
        }
    }
}
