#include "engine/quat.h"

#include <cassert>
#include <cmath>

#include "engine/angle.h"
#include "engine/axisangle.h"
#include "engine/fileutil.h"
#include "engine/mat33.h"
#include "engine/math.h"
#include "engine/vec3.h"
#include "fmt/core.h"

namespace simple
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

    quat quat::FromYawPitchRoll(const Angle& yaw, const Angle& pitch, const Angle& roll)
    {
        // Abbreviations for the various angular functions
        const auto cy = (yaw * 0.5f).Cos();
        const auto sy = (yaw * 0.5f).Sin();
        const auto cp = (pitch * 0.5f).Cos();
        const auto sp = (pitch * 0.5f).Sin();
        const auto cr = (roll * 0.5f).Cos();
        const auto sr = (roll * 0.5f).Sin();

        const auto w = cr * cp * cy + sr * sp * sy;
        const auto x = sr * cp * cy - cr * sp * sy;
        const auto y = cr * sp * cy + sr * cp * sy;
        const auto z = cr * cp * sy - sr * sp * cy;

        return quat(vec3(x, y, z), w);
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

    vec3 quat::GetVec() const
    {
        return vec3(x, y, z);
    }

    void quat::SetVec(const vec3& value)
    {
        x = value.x;
        y = value.y;
        z = value.z;
    }

    quat quat::Conjugate() const
    {
        return quat(w, -GetVec());
    }

    quat quat::AllNegative() const
    {
        return quat(-w, -GetVec());
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

    float quat::GetLength() const
    {
        return (float)std::sqrt(GetLengthSquared());
    }

    float quat::GetLengthSquared() const
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
        d = math::Within(-1, d, 1);
        Angle theta0 = Angle::Acos(d);
        Angle theta = theta0 * v;

        quat q = (b - a * d).GetNormalized();
        return a * theta.Cos() + q * theta.Sin();
    }

    quat quat::lerp(const quat& a, float v, const quat& b)
    {
        return (a + v * (b - a)).GetNormalized();
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

    bool IsZero(float f)
    {
        return std::abs(f) < 0.006;
    }

    AxisAngle quat::GetAxisAngle() const
    {
        if (IsZero(x) && IsZero(y) && IsZero(z))
        {
            return AxisAngle::RightHandAround(vec3::In(), Angle::FromRadians(0));
        }
        else
        {
            return AxisAngle::RightHandAround(GetVec().GetNormalized(), Angle::Acos(w) * 2);
        }
    }

    quat::quat(const simple::AxisAngle& aa)
    {
        Angle half = aa.angle * 0.5f;
        SetVec(aa.axis * half.Sin());
        w = half.Cos();
        Normalize();
    }

    void quat::Normalize()
    {
        float l = GetLength();
        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    quat quat::GetNormalized() const
    {
        quat q = quat(*this);
        q.Normalize();
        return q;
    }

    vec3 quat::GetRUI(const vec3& rui) const
    {
        return Right() * rui.x + Up() * rui.y + In() * rui.z;
    }

    mat33 quat::GetMatrix33() const
    {
        float tXX = 2 * math::Square(x);
        float tYY = 2 * math::Square(y);
        float tZZ = 2 * math::Square(z);
        float tXY = 2 * x * y;
        float tYZ = 2 * y * z;
        float tZW = 2 * z * w;
        float tXW = 2 * x * w;
        float tXZ = 2 * x * z;
        float tYW = 2 * y * w;
        return mat33::FromRowMajor(mat33::FA{1 - tYY - tZZ, tXY - tZW, tXZ + tYW, tXY + tZW, 1 - tXX - tZZ, tYZ - tXW,
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
        quat rx = quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(-dx)));
        quat ry = quat(AxisAngle::RightHandAround(rotation.Right(), Angle::FromDegrees(-dy)));
        quat finalq = rx * ry;
        return finalq;
    }

    quat operator-(const quat& l, const quat& r)
    {
        return quat(l.w - r.w, l.GetVec() - r.GetVec());
    }

    quat operator+(const quat& l, const quat& r)
    {
        return quat(l.w + r.w, l.GetVec() + r.GetVec());
    }

    quat operator*(const quat& l, float r)
    {
        return quat(l.w * r, l.GetVec() * r);
    }
    quat operator*(float r, const quat& l)
    {
        return quat(l.w * r, l.GetVec() * r);
    }

    quat operator*(const quat& l, const quat& r)
    {
        return quat(r.GetVec() * l.w + l.GetVec() * r.w + vec3::cross(l.GetVec(), r.GetVec()),
                    l.w * r.w - vec3::dot(l.GetVec(), r.GetVec()));
    }
    quat operator-(const quat& me)
    {
        return me.Conjugate();
    }
}
