#include "engine/quat.h"

#include <cassert>
#include <cmath>

#include "engine/axisangle.h"
#include "engine/fileutil.h"
#include "engine/mat33.h"
#include "engine/math1.h"
#include "engine/vec3.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    std::string quat::ToString() const
    {
        return fmt::format("({} {} {} {})", x, y, z, w);
    }

    quat::quat()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(1.0f)
    {
    }

    quat::quat(float ww, const vec3& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(ww)
    {
    }

    quat::quat(const vec3& v, float ww)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(ww)
    {
    }

    quat::quat(const quat& other)
        : x(other.x)
        , y(other.y)
        , z(other.z)
        , w(other.w)
    {
    }

    quat quat::Read(BinaryReader& br)
    {
        const auto x = br.ReadSingle();
        const auto y = br.ReadSingle();
        const auto z = br.ReadSingle();
        const auto w = br.ReadSingle();

        return quat{vec3{x, y, z}, w};
    }

    void quat::Write(const quat& q, BinaryWriter& br)
    {
        br.WriteSingle(q.x);
        br.WriteSingle(q.y);
        br.WriteSingle(q.z);
        br.WriteSingle(q.w);
    }

    vec3 quat::vec() const
    {
        return vec3(x, y, z);
    }

    void quat::vec(const vec3& value)
    {
        x = value.x;
        y = value.y;
        z = value.z;
    }

    quat quat::Conjugate() const
    {
        return quat(w, -vec());
    }

    quat quat::AllNegative() const
    {
        return quat(-w, -vec());
    }

    quat quat::Combine(const quat& current, const quat& extra)
    {
        return extra * current;
    }

    vec3 quat::In() const
    {
        return vec3::RotateAroundOrigo(*this, vec3::In());
    }
    vec3 quat::Right() const
    {
        return vec3::RotateAroundOrigo(*this, vec3::Right());
    }
    vec3 quat::Up() const
    {
        return vec3::RotateAroundOrigo(*this, vec3::Up());
    }

    vec3 quat::Out() const
    {
        return vec3::RotateAroundOrigo(*this, vec3::Out());
    }
    vec3 quat::Left() const
    {
        return vec3::RotateAroundOrigo(*this, vec3::Left());
    }
    vec3 quat::Down() const
    {
        return vec3::RotateAroundOrigo(*this, vec3::Down());
    }

    quat quat::Identity()
    {
        return quat(1, vec3::Zero());
    }

    float quat::Length() const
    {
        return (float)std::sqrt(LengthSquared());
    }

    float quat::LengthSquared() const
    {
        return x * x + y * y + z * z + w * w;
    }

    quat quat::Slerp(const quat& a, float v, const quat& b)
    {
        float d = dot(a, b);
        if (d > 0.9995f)
        {
            return lerp(a, v, b);
        }
        d = math1::Within(-1, d, 1);
        angle theta0 = angle::Acos(d);
        angle theta = theta0 * v;

        quat q = (b - a * d).Normalized();
        return a * theta.Cos() + q * theta.Sin();
    }

    quat quat::lerp(const quat& a, float v, const quat& b)
    {
        return (a + v * (b - a)).Normalized();
    }

    float quat::dot(const quat& A, const quat& B)
    {
        return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
    }

    // forces the interpolatation to go the "short way"
    quat quat::SlerpShortway(const quat& a, float t, const quat& b)
    {
        if (dot(a, b) < 0)
            return Slerp(a.AllNegative(), t, b);
        else
            return Slerp(a, t, b);
    }

    bool isZero(float f)
    {
        return std::abs(f) < 0.006;
    }

    AxisAngle quat::AxisAngle() const
    {
        if (isZero(x) && isZero(y) && isZero(z))
        {
            return AxisAngle::RightHandAround(vec3::In(), angle::FromRadians(0));
        }
        else
        {
            return AxisAngle::RightHandAround(vec().Normalized(), angle::Acos(w) * 2);
        }
    }

    quat::quat(const SimpleEngine::AxisAngle& aa)
    {
        angle half = aa.angle * 0.5f;
        vec(aa.axis * half.Sin());
        w = half.Cos();
        normalize();
    }

    void quat::normalize()
    {
        float l = Length();
        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    quat quat::Normalized() const
    {
        quat q = quat(*this);
        q.normalize();
        return q;
    }

    vec3 quat::getRUI(const vec3& rui) const
    {
        return Right() * rui.x + Up() * rui.y + In() * rui.z;
    }

    mat33 quat::mat33() const
    {
        float tXX = 2 * math1::Square(x);
        float tYY = 2 * math1::Square(y);
        float tZZ = 2 * math1::Square(z);
        float tXY = 2 * x * y;
        float tYZ = 2 * y * z;
        float tZW = 2 * z * w;
        float tXW = 2 * x * w;
        float tXZ = 2 * x * z;
        float tYW = 2 * y * w;
        return mat33::FromRowMajor(mat33::FA{
            1 - tYY - tZZ, tXY - tZW, tXZ + tYW,
            tXY + tZW, 1 - tXX - tZZ, tYZ - tXW,
            tXZ - tYW, tYZ + tXW, 1 - tXX - tYY});
    }

    float quat::operator()(int index) const
    {
        if (index == 0)
            return x;
        else if (index == 1)
            return y;
        else if (index == 2)
            return z;
        else if (index == 3)
            return w;
        else
            assert(false && "bad index");
        return x;
    }
    float& quat::operator()(int index)
    {
        if (index == 0)
            return x;
        else if (index == 1)
            return y;
        else if (index == 2)
            return z;
        else if (index == 3)
            return w;
        else
            assert(false && "bad index");
        return x;
    }

    quat quat::FpsQuat(const quat& rotation, float dx, float dy)
    {
        quat rx = quat(AxisAngle::RightHandAround(vec3::Up(), angle::FromDegrees(-dx)));
        quat ry = quat(AxisAngle::RightHandAround(rotation.Right(), angle::FromDegrees(-dy)));
        quat finalq = rx * ry;
        return finalq;
    }

    quat operator-(const quat& l, const quat& r)
    {
        return quat(l.w - r.w, l.vec() - r.vec());
    }

    quat operator+(const quat& l, const quat& r)
    {
        return quat(l.w + r.w, l.vec() + r.vec());
    }

    quat operator*(const quat& l, float r)
    {
        return quat(l.w * r, l.vec() * r);
    }
    quat operator*(float r, const quat& l)
    {
        return quat(l.w * r, l.vec() * r);
    }

    quat operator*(const quat& l, const quat& r)
    {
        return quat(r.vec() * l.w + l.vec() * r.w + vec3::cross(l.vec(), r.vec()),
                    l.w * r.w - vec3::dot(l.vec(), r.vec()));
    }
    quat operator-(const quat& me)
    {
        return me.Conjugate();
    }
}
