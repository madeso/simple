using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization;

namespace SimpleEngine
{
    public static class math1
    {
        // larger smoothing values result in a smoother but less resposive path
        public static float Curve(float newValue, float oldValue, float smoothingValue)
        {
            int sign = Math.Sign(oldValue - newValue);
            float slip = (oldValue - newValue) / smoothingValue;
            float val = oldValue - slip;
            if (sign != Math.Sign(val - newValue)) return newValue;
            else return val;
        }

        private static NumberFormatInfo nf = null;
        public static float ParseFloat(string text)
        {
            if (nf == null)
            {
                nf = System.Globalization.CultureInfo.CreateSpecificCulture("en").NumberFormat;
            }
            return float.Parse(text, nf);
        }

        public static bool IsZero(float l)
        {
            return Abs(l) < 0.001f;
        }

        public static float Square(float r)
        {
            return r * r;
        }
        public static int Square(int r)
        {
            return r * r;
        }

        public static float Sqrt(float r)
        {
            return (float)Math.Sqrt(r);
        }

        public static float Abs(float r)
        {
            return Math.Abs(r);
        }

        public static float Min(float lhs, float rhs)
        {
            if (lhs < rhs) return lhs;
            else return rhs;
        }

        public static float Max(float lhs, float rhs)
        {
            if (lhs > rhs) return lhs;
            else return rhs;
        }

        public static float Limmit(float min, float value, float max)
        {
            return Max(min, Min(max, value));
        }

        public static float To01(float L, float v, float R)
        {
            return (v - L) / (R - L);
        }
        public static float From01(float L, float v, float R)
        {
            return v * (R - L) + L;
        }
        public static float Remap(float ol, float or, float v, float nl, float nr)
        {
            return From01(nl, To01(ol, v, or), nr);
        }


        public static float Get360Angular(float min, float value, float max)
        {
            float halfdiff = (max - min) / 2;
            return min + halfdiff - halfdiff * angle.FromDegrees(value * 360).Cos;
        }

        public static float Within(float min, float v, float max)
        {
            if (v > max) return max;
            else if (v < min) return min;
            else return v;
        }
        public static bool IsWithin(float min, float c, float max)
        {
            return c >= min && c <= max;
        }

        public static float Wrap(float min, float v, float max)
        {
            float diff = max - min;
            float value = v - min;
            while (value < 0) value += diff;
            while (value > diff) value -= diff;
            return min + value;
        }
        public static int Wrapi(int min, int v, int max)
        {
            int diff = max - min + 1;
            int value = v;
            while (value < min) value += diff;
            while (value > max) value -= diff;
            return value;
        }



        // return number of wraps (with sign)
        public static int IncrementAndWrap(float min, ref float current, float change, float max)
        {
            float diff = max - min;
            float value = current + change;
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
            current = value;
            return wraps;
        }

        public static int IncrementAndWrapi(int min, ref int current, int change, int max)
        {
            int diff = max - min + 1;
            int value = current + change;
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
            current = value;
            return wraps;
        }

        public static bool IsBetween(int start, float p, int end)
        {
            return start <= p && p <= end;
        }

        public static bool isSame(float length, float last)
        {
            return IsZero(length - last);
        }

        internal static float ZeroOrValue(float mark)
        {
            if (IsZero(mark)) return 0;
            else return mark;
        }
    }
}
