#pragma once

#include <string>

namespace simple
{
    struct Angle;
    struct quat;
    struct BinaryReader;
    struct BinaryWriter;

    struct vec3
    {
        float x, y, z;

        std::string ToString() const;

        vec3(float ax = 0.0f, float ay = 0.0f, float az = 0.0f);

        vec3(const vec3& a);

        static vec3 Read(BinaryReader& br);
        static void Write(const vec3& v, BinaryWriter& br);

        // override string ToString()
        // {
        //     return string.Format("({0} {1} {2})", x, y, z);
        // }

        static vec3 Zero();

        float operator()(int index) const;

        vec3 Scale(float s) const;

        vec3 Negative() const;

        quat ToQuat() const;

        static vec3 FromTo(const vec3& from, const vec3& to);

        static vec3 cross(const vec3& lhs, const vec3& rhs);

        static float dot(const vec3& lhs, const vec3& rhs);

        static vec3 RotateAroundOrigo(const simple::quat& q, const vec3& v);

        float GetLength() const;

        float GetLengthSquared() const;

        void Normalize();

        vec3 GetNormalized() const;

        static vec3 Right();
        static vec3 In();
        static vec3 Up();

        static vec3 Left();
        static vec3 Out();
        static vec3 Down();

        static vec3 Curve(const vec3& target, const vec3& old, float smoothing);

        static Angle AngleBetween(const vec3& f, const vec3& t);

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
