#include "engine/math1.h"

#include <cmath>

#include "engine/angle.h"

namespace SimpleEngine
{
    namespace math1
    {
        int Sign(float s)
        {
            return s > 0.0f ? 1 : -1;
        }

        float Curve(float newValue, float oldValue, float smoothingValue)
        {
            int sign = Sign(oldValue - newValue);
            float slip = (oldValue - newValue) / smoothingValue;
            float val = oldValue - slip;
            if (sign != Sign(val - newValue))
                return newValue;
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

        float To01(float L, float v, float R)
        {
            return (v - L) / (R - L);
        }

        float From01(float L, float v, float R)
        {
            return v * (R - L) + L;
        }

        float Remap(float ol, float oor, float v, float nl, float nr)
        {
            return From01(nl, To01(ol, v, oor), nr);
        }

        float Get360Angular(float min, float value, float max)
        {
            float halfdiff = (max - min) / 2;
            return min + halfdiff - halfdiff * angle::FromDegrees(value * 360).Cos();
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

        bool isSame(float length, float last)
        {
            return IsZero(length - last);
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
