#include "engine/math.h"

#include <cmath>

#include "engine/angle.h"

namespace simple
{
    namespace math
    {
        int Sign(float s)
        {
            return s > 0.0f ? 1 : -1;
        }

        float Curve(float new_value, float old_value, float smoothing_value)
        {
            int sign = Sign(old_value - new_value);
            float slip = (old_value - new_value) / smoothing_value;
            float val = old_value - slip;
            if (sign != Sign(val - new_value))
                return new_value;
            else
                return val;
        }

        float ParseFloat(const std::string& text)
        {
            return std::stof(text);
        }

        bool IsZero(float l)
        {
            return Abs(l) < 0.001f;
        }

        float Square(float r)
        {
            return r * r;
        }

        int Square(int r)
        {
            return r * r;
        }

        float Sqrt(float r)
        {
            return (float)std::sqrt(r);
        }

        float Abs(float r)
        {
            return std::abs(r);
        }

        float Min(float lhs, float rhs)
        {
            if (lhs < rhs)
                return lhs;
            else
                return rhs;
        }

        float Max(float lhs, float rhs)
        {
            if (lhs > rhs)
                return lhs;
            else
                return rhs;
        }

        float Limmit(float min, float value, float max)
        {
            return Max(min, Min(max, value));
        }

        float To01(float min, float v, float max)
        {
            return (v - min) / (max - min);
        }

        float From01(float min, float v, float max)
        {
            return v * (max - min) + min;
        }

        float Remap(float ol, float oor, float v, float nl, float nr)
        {
            return From01(nl, To01(ol, v, oor), nr);
        }

        float Get360Angular(float min, float value, float max)
        {
            float halfdiff = (max - min) / 2;
            return min + halfdiff - halfdiff * Angle::FromDegrees(value * 360).Cos();
        }

        float Within(float min, float v, float max)
        {
            if (v > max)
                return max;
            else if (v < min)
                return min;
            else
                return v;
        }

        bool IsWithin(float min, float c, float max)
        {
            return c >= min && c <= max;
        }

        float Wrap(float min, float v, float max)
        {
            float diff = max - min;
            float value = v - min;
            while (value < 0) value += diff;
            while (value > diff) value -= diff;
            return min + value;
        }

        int Wrapi(int min, int v, int max)
        {
            int diff = max - min + 1;
            int value = v;
            while (value < min) value += diff;
            while (value > max) value -= diff;
            return value;
        }

        // return number of wraps (with sign)
        int IncrementAndWrap(float min, float* current, float change, float max)
        {
            float diff = max - min;
            float value = *current + change;
            int wraps = 0;
            while (value < min)
            {
                value += diff;
                --wraps;
            }
            while (value > max)
            {
                value -= diff;
                ++wraps;
            }
            *current = value;
            return wraps;
        }

        int IncrementAndWrapi(int min, int* current, int change, int max)
        {
            int diff = max - min + 1;
            int value = *current + change;
            int wraps = 0;
            while (value < min)
            {
                value += diff;
                --wraps;
            }
            while (value > max)
            {
                value -= diff;
                ++wraps;
            }
            *current = value;
            return wraps;
        }

        bool IsBetween(int start, float p, int end)
        {
            return start <= p && p <= end;
        }

        bool IsSame(float lhs, float rhs)
        {
            return IsZero(lhs - rhs);
        }

        float ZeroOrValue(float mark)
        {
            if (IsZero(mark))
                return 0;
            else
                return mark;
        }
    }
}
