﻿#pragma once

#include <string>

namespace SimpleEngine
{
    struct vec2
    {
        float x, y;

        vec2(float xx, float yy);

        static vec2 Zero();

        std::string ToString() const;

        vec2 scale(float s) const;

        static vec2 FromTo(const vec2& from, const vec2& to);

        static vec2 Curve(const vec2& target, const vec2& old, float smoothing);
    };

    vec2 operator+(const vec2& lhs, const vec2& rhs);

    vec2 operator-(const vec2& lhs, const vec2& rhs);

    vec2 operator*(const vec2& lhs, float rhs);
}
