using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class vec3
    {
        public float x, y, z;

        public vec3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        public vec3(vec3 copy)
        {
            this.x = copy.x;
            this.y = copy.y;
            this.z = copy.z;
        }

        public override string ToString()
        {
            return string.Format("({0} {1} {2})", x, y, z);
        }

        public static vec3 operator + (vec3 lhs, vec3 rhs)
        {
            return new vec3(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z);
        }
        public static vec3 operator- (vec3 lhs, vec3 rhs)
        {
            return new vec3(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z);
        }
        public static vec3 operator *(vec3 lhs, float rhs)
        {
            return lhs.scale(rhs);
        }
        public static vec3 Zero
        {
            get
            {
                return new vec3(0, 0, 0);
            }
        }

        public float this[int index]
        {
            get
            {
                if (index == 0) return x;
                else if (index == 1) return y;
                else if (index == 2) return z;
                else throw new ArgumentException("index is bad: " + index.ToString());
            }
        }

        public vec3 scale(float s)
        {
            return new vec3(x * s, y * s, z * s);
        }
        public static vec3 operator-(vec3 me)
        {
            return me.Negative;
        }

        public vec3 Negative
        {
            get
            {
                return new vec3(-x, -y, -z);
            }
        }

        public quat quat
        {
            get
            {
                return new quat(this, 0);
            }
        }

        public static vec3 FromTo(vec3 from, vec3 to)
        {
            return to - from;
        }

        public static vec3 cross(vec3 lhs, vec3 rhs)
	    {
		    vec3 A = lhs;
		    vec3 B = rhs;
		    return new vec3(A.y*B.z - A.z*B.y, A.z*B.x - A.x*B.z, A.x*B.y - A.y*B.x);
	    }

	    public static float dot(vec3 lhs, vec3 rhs)
        {
	        vec3 A = lhs;
	        vec3 B = rhs;
	        return A.x * B.x + A.y * B.y + A.z * B.z;
        }

        public static vec3 RotateAroundOrigo(quat q, vec3 v)
        {
            quat r = q * v.quat * q.Conjugate;
            return r.vec;
        }

        public float Length
        {
            get
            {
                return (float) Math.Sqrt(LengthSquared);
            }
        }
        public float LengthSquared
        {
            get
            {
                return x * x + y * y + z * z;
            }
        }

        public void normalize()
        {
            float l = Length;
            if (math1.IsZero(l)) return;
            x /= l;
            y /= l;
            z /= l;
        }

        public vec3 Normalized
        {
            get
            {
                vec3 copy = new vec3(this);
                copy.normalize();
                return copy;
            }
        }

        public static vec3 Right
        {
            get
            {
                return new vec3(1, 0, 0);
            }
        }
        public static vec3 In
        {
            get
            {
                return new vec3(0, 0, -1);
            }
        }
        public static vec3 Up
        {
            get
            {
                return new vec3(0, 1, 0);
            }
        }

        public static vec3 Left
        {
            get
            {
                return -Right;
            }
        }
        public static vec3 Out
        {
            get
            {
                return -In;
            }
        }
        public static vec3 Down
        {
            get
            {
                return -Up;
            }
        }

        public static vec3 Curve(vec3 target, vec3 old, float smoothing)
        {
            return new vec3(math1.Curve(target.x, old.x, smoothing), math1.Curve(target.y, old.y, smoothing), math1.Curve(target.z, old.z, smoothing) );
        }

        internal static angle AngleBetween(vec3 f, vec3 t)
        {
            return angle.Acos(vec3.dot(f, t));
        }

        internal static vec3 Read(System.IO.BinaryReader br)
        {
            float x = br.ReadSingle();
            float y = br.ReadSingle();
            float z = br.ReadSingle();
            return new vec3(x, y, z);
        }

        internal static void Write(vec3 vec, System.IO.BinaryWriter bw)
        {
            bw.Write(vec.x);
            bw.Write(vec.y);
            bw.Write(vec.z);
        }

        public static vec3 Interpolate(vec3 f, float scale, vec3 t)
        {
            return f + (t - f) * scale;
        }
    }
}
