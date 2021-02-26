#include "test.h"

#include <cmath>

#include "engine/axisangle.h"
#include "engine/mat33.h"
#include "engine/mat44.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/vec3.h"
#include "fmt/core.h"

namespace simple
{
    namespace
    {
        constexpr float fs = 0.001f;

        bool IsClose(float a, float b)
        {
            return std::abs(a - b) < fs;
        }
        bool IsClose(const quat& a, const quat& b)
        {
            return IsClose(a.x, b.x) && IsClose(a.y, b.y) && IsClose(a.z, b.z) && IsClose(a.w, b.w);
        }
        bool IsClose(const vec3& a, const vec3& b)
        {
            return IsClose(a.x, b.x) && IsClose(a.y, b.y) && IsClose(a.z, b.z);
        }
        bool IsClose(const vec2& a, const vec2& b)
        {
            return IsClose(a.x, b.x) && IsClose(a.y, b.y);
        }
        bool IsClose(const AxisAngle& a, const AxisAngle& b)
        {
            return IsClose(a.axis, b.axis) && IsClose(a.angle.InDegrees(), b.angle.InDegrees());
        }
        bool IsClose(const mat33& a, const mat33& b)
        {
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    if (IsClose(a(i, j), b(i, j)) == false)
                        return false;
                }
            }
            return true;
        }

        bool IsClose(const mat44& a, const mat44& b)
        {
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    if (IsClose(a(i, j), b(i, j)) == false)
                        return false;
                }
            }
            return true;
        }

        bool Matches(const vec3& q, const vec3& act)
        {
            return IsClose(act, q);
        }

        bool Matches(const quat& q, const quat& act)
        {
            return IsClose(act, q);
        }

        bool Matches(const AxisAngle& a, const AxisAngle& act)
        {
            if (IsClose(act.angle.InDegrees(), 0) && IsClose(a.angle.InDegrees(), 0))
                return true;
            return IsClose(act, a) || IsClose(-act, a);
        }

        bool Matches(const mat33& a, const mat33& act)
        {
            return IsClose(act, a);
        }

        bool Matches(const mat44& a, const mat44& act)
        {
            return IsClose(act, a);
        }
    }

    catchy::FalseString AreEqual(float a, float b)
    {
        if (IsClose(a, b))
        {
            return catchy::FalseString::True();
        }
        else
        {
            return catchy::FalseString::False(fmt::format("{} != {}", a, b));
        }
    }

    catchy::FalseString AreEqual(const quat& a, const quat& b)
    {
        if (Matches(a, b))
        {
            return catchy::FalseString::True();
        }
        else
        {
            return catchy::FalseString::False(fmt::format("{} != {}", a.ToString(), b.ToString()));
        }
    }

    catchy::FalseString AreEqual(const vec3& a, const vec3& b)
    {
        if (Matches(a, b))
        {
            return catchy::FalseString::True();
        }
        else
        {
            return catchy::FalseString::False(fmt::format("{} != {}", a.ToString(), b.ToString()));
        }
    }

    catchy::FalseString AreEqual(const AxisAngle& a, const AxisAngle& b)
    {
        if (Matches(a, b))
        {
            return catchy::FalseString::True();
        }
        else
        {
            return catchy::FalseString::False(fmt::format("{} != {}", a.ToString(), b.ToString()));
        }
    }

    catchy::FalseString AreEqual(const mat33& a, const mat33& b)
    {
        if (Matches(a, b))
        {
            return catchy::FalseString::True();
        }
        else
        {
            return catchy::FalseString::False(fmt::format("{} != {}", a.ToString(), b.ToString()));
        }
    }

    catchy::FalseString AreEqual(const mat44& a, const mat44& b)
    {
        if (Matches(a, b))
        {
            return catchy::FalseString::True();
        }
        else
        {
            return catchy::FalseString::False(fmt::format("{} != {}", a.ToString(), b.ToString()));
        }
    }

}
