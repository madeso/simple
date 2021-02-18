using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public struct angle
    {
        private float rad;
        private angle(float r)
        {
            this.rad = r;
        }

        public float inRadians
        {
            get
            {
                return rad;
            }
        }

        private static float rad2deg(float rad)
		{
			return (float) (180/Math.PI) * rad;
		}
        private static float deg2rad(float deg)
		{
            return (float) Math.PI / 180 * deg;
		}

        public float inDegrees
        {
            get
            {
                return rad2deg(rad);
            }
        }

        public static angle FromRadians(float r)
        {
            return new angle(r);
        }
        public static angle FromDegrees(float deg)
        {
            return new angle(deg2rad(deg));
        }
        public static angle operator +(angle l, angle r)
        {
            return angle.FromRadians(l.inRadians + r.inRadians);
        }

        public static angle Zero
        {
            get
            {
                return angle.FromRadians(0);
            }
        }
        public static angle FromPercentOf360(float percent)
        {
            return angle.FromRadians( (float)( percent * Math.PI * 2));
        }

        public static angle operator -(angle l, angle r)
        {
            return angle.FromRadians(l.inRadians - r.inRadians);
        }
        public static angle operator *(angle l, float r)
        {
            return angle.FromRadians(l.inRadians * r);
        }
        public static angle operator -(angle me)
        {
            return me.Negative;
        }

        public angle Negative
        {
            get
            {
                return new angle(-rad);
            }
        }

        public float Sin
        {
            get
            {
                return (float)Math.Sin(inRadians);
            }
        }
        public float Cos
        {
            get
            {
                return (float) Math.Cos(inRadians);
            }
        }
        public float Tan
        {
            get
            {
                return (float)Math.Tan(inRadians);
            }
        }

        public static angle Asin(float v)
        {
            return angle.FromRadians( (float) Math.Asin(v) );
        }

        public static angle Acos(float v)
        {
            return angle.FromRadians((float)Math.Acos(v));
        }

        public static angle Atan(float v)
        {
            return angle.FromRadians((float)Math.Atan(v));
        }
    }
}
