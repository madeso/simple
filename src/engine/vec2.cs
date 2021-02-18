using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class vec2
    {
        public float x, y;
        public vec2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public static vec2 Zero
        {
            get
            {
                return new vec2(0, 0);
            }
        }

        public override string ToString()
        {
            return string.Format("({0} {1})", x, y);
        }

        public static vec2 operator +(vec2 lhs, vec2 rhs)
        {
            return new vec2(lhs.x + rhs.x, lhs.y + rhs.y);
        }
        public static vec2 operator -(vec2 lhs, vec2 rhs)
        {
            return new vec2(lhs.x - rhs.x, lhs.y - rhs.y);
        }
        public static vec2 operator *(vec2 lhs, float rhs)
        {
            return lhs.scale(rhs);
        }

        private vec2 scale(float s)
        {
            return new vec2(x*s, y*s);
        }

        public static vec2 FromTo(vec2 from, vec2 to)
        {
            return to - from;
        }

        public static vec2 Curve(vec2 target, vec2 old, float smoothing)
        {
            return new vec2(math1.Curve(target.x, old.x, smoothing), math1.Curve(target.y, old.y, smoothing));
        }
    }
}
