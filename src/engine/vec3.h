#pragma once

#include <string>

namespace SimpleEngine
{
    struct angle;
    struct quat;
    struct BinaryReader;

    struct vec3
    {
        float x, y, z;

        std::string ToString() const;

        vec3(float ax = 0.0f, float ay = 0.0f, float az = 0.0f);

        vec3(const vec3& a);

        static vec3 Read(BinaryReader& br);

        // override string ToString()
        // {
        //     return string.Format("({0} {1} {2})", x, y, z);
        // }

        static vec3 Zero();

        float operator()(int index) const;

        vec3 scale(float s) const;

        vec3 Negative() const;

        quat quat() const;

        static vec3 FromTo(const vec3& from, const vec3& to);

        static vec3 cross(const vec3& lhs, const vec3& rhs);

        static float dot(const vec3& lhs, const vec3& rhs);

        static vec3 RotateAroundOrigo(const SimpleEngine::quat& q, const vec3& v);

        float Length() const;

        float LengthSquared() const;

        void normalize();

        vec3 Normalized() const;

        static vec3 Right();
        static vec3 In();
        static vec3 Up();

        static vec3 Left();
        static vec3 Out();
        static vec3 Down();

        static vec3 Curve(const vec3& target, const vec3& old, float smoothing);

        static angle AngleBetween(const vec3& f, const vec3& t);

        // static vec3 Read(System.IO.BinaryReader br)
        // {
        //     float x = br.ReadSingle();
        //     float y = br.ReadSingle();
        //     float z = br.ReadSingle();
        //     return vec3(x, y, z);
        // }
        // static void Write(vec3 vec, System.IO.BinaryWriter bw)
        // {
        //     bw.Write(vec.x);
        //     bw.Write(vec.y);
        //     bw.Write(vec.z);
        // }

        static vec3 Interpolate(const vec3& f, float scale, const vec3& t);
    };

    vec3 operator+(const vec3& lhs, const vec3& rhs);
    vec3 operator-(const vec3& lhs, const vec3& rhs);
    vec3 operator*(const vec3& lhs, float rhs);

    vec3 operator-(const vec3& me);
}
