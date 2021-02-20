#include "engine/vec3.h"

#include <cassert>
#include <cmath>

#include "engine/angle.h"
#include "engine/fileutil.h"
#include "engine/math1.h"
#include "engine/quat.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    std::string vec3::ToString() const
    {
        return fmt::format("({} {} {})", x, y, z);
    }

    vec3::vec3(float ax, float ay, float az)
        : x(ax)
        , y(ay)
        , z(az)
    {
    }

    vec3::vec3(const vec3& a)
        : x(a.x)
        , y(a.y)
        , z(a.z)
    {
    }

    vec3 vec3::Read(BinaryReader& br)
    {
        auto x = br.ReadSingle();
        auto y = br.ReadSingle();
        auto z = br.ReadSingle();
        return vec3{x, y, z};
    }

    void vec3::Write(const vec3& v, BinaryWriter& br)
    {
        br.WriteSingle(v.x);
        br.WriteSingle(v.y);
        br.WriteSingle(v.z);
    }

    vec3 vec3::Zero()
    {
        return vec3(0, 0, 0);
    }

    float vec3::operator()(int index) const
    {
        if (index == 0)
            return x;
        else if (index == 1)
            return y;
        else if (index == 2)
            return z;
        else
        {
            assert(false && "index is bad");
            return 0.0f;
        }
    }

    vec3 vec3::scale(float s) const
    {
        return vec3(x * s, y * s, z * s);
    }

    vec3 vec3::Negative() const
    {
        return vec3(-x, -y, -z);
    }

    SimpleEngine::quat vec3::quat() const
    {
        return SimpleEngine::quat(*this, 0);
    }

    vec3 vec3::FromTo(const vec3& from, const vec3& to)
    {
        return to - from;
    }

    vec3 vec3::cross(const vec3& lhs, const vec3& rhs)
    {
        const vec3& A = lhs;
        const vec3& B = rhs;
        return vec3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
    }

    float vec3::dot(const vec3& lhs, const vec3& rhs)
    {
        const vec3& A = lhs;
        const vec3& B = rhs;
        return A.x * B.x + A.y * B.y + A.z * B.z;
    }

    vec3 vec3::RotateAroundOrigo(const SimpleEngine::quat& q, const vec3& v)
    {
        SimpleEngine::quat r = q * v.quat() * q.Conjugate();
        return r.vec();
    }

    float vec3::Length() const
    {
        return (float)std::sqrt(LengthSquared());
    }

    float vec3::LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    void vec3::normalize()
    {
        float l = Length();
        if (math1::IsZero(l))
            return;
        x /= l;
        y /= l;
        z /= l;
    }

    vec3 vec3::Normalized() const
    {
        vec3 copy = *this;
        copy.normalize();
        return copy;
    }

    vec3 vec3::Right()
    {
        return vec3(1, 0, 0);
    }
    vec3 vec3::In()
    {
        return vec3(0, 0, -1);
    }
    vec3 vec3::Up()
    {
        return vec3(0, 1, 0);
    }

    vec3 vec3::Left()
    {
        return -Right();
    }
    vec3 vec3::Out()
    {
        return -In();
    }
    vec3 vec3::Down()
    {
        return -Up();
    }

    vec3 vec3::Curve(const vec3& target, const vec3& old, float smoothing)
    {
        return vec3(math1::Curve(target.x, old.x, smoothing), math1::Curve(target.y, old.y, smoothing), math1::Curve(target.z, old.z, smoothing));
    }

    angle vec3::AngleBetween(const vec3& f, const vec3& t)
    {
        return angle::Acos(vec3::dot(f, t));
    }

    vec3 vec3::Interpolate(const vec3& f, float scale, const vec3& t)
    {
        return f + (t - f) * scale;
    }

    vec3 operator+(const vec3& lhs, const vec3& rhs)
    {
        return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }
    vec3 operator-(const vec3& lhs, const vec3& rhs)
    {
        return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }
    vec3 operator*(const vec3& lhs, float rhs)
    {
        return lhs.scale(rhs);
    }

    vec3 operator-(const vec3& me)
    {
        return me.Negative();
    }
}
