#pragma once

#include <string>

namespace SimpleEngine
{
    struct vec3;
    struct AxisAngle;
    struct mat33;

    struct quat
    {
        float x;
        float y;
        float z;
        float w;

        std::string ToString() const;

        quat();
        quat(float w, const vec3& vec);
        quat(const vec3& v, float w);
        quat(const quat& other);

        vec3 vec() const;
        void vec(const vec3& v);

        quat Conjugate() const;

        quat AllNegative() const;

        static quat Combine(const quat& current, const quat& extra);

        vec3 In() const;
        vec3 Right() const;
        vec3 Up() const;

        vec3 Out() const;
        vec3 Left() const;
        vec3 Down() const;

        static quat Identity();

        float Length() const;

        float LengthSquared() const;

        static quat Slerp(const quat& a, float v, const quat& b);

        static quat lerp(const quat& a, float v, const quat& b);

        static float dot(const quat& A, const quat& B);

        // forces the interpolatation to go the "short way"
        static quat SlerpShortway(const quat& a, float t, const quat& b);

        AxisAngle AxisAngle() const;

        quat(const SimpleEngine::AxisAngle& aa);

        void normalize();

        quat Normalized() const;

        vec3 getRUI(const vec3& rui) const;

        SimpleEngine::mat33 mat33() const;

        float operator()(int index) const;
        float& operator()(int index);

        static quat FpsQuat(const quat& rotation, float dx, float dy);

        // static void Write(quat quat, System.IO.BinaryWriter bw)
        // {
        //     bw.Write(quat.x);
        //     bw.Write(quat.y);
        //     bw.Write(quat.z);
        //     bw.Write(quat.w);
        // }
        // static quat Read(System.IO.BinaryReader br)
        // {
        //     float x = br.ReadSingle();
        //     float y = br.ReadSingle();
        //     float z = br.ReadSingle();
        //     float w = br.ReadSingle();
        //     return quat(w, vec3(x, y, z));
        // }
    };

    quat operator-(const quat& l, const quat& r);
    quat operator+(const quat& l, const quat& r);

    quat operator*(const quat& l, float r);
    quat operator*(float r, const quat& l);

    quat operator*(const quat& l, const quat& r);
    quat operator-(const quat& me);
}
