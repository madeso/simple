using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class quat
    {
        public float x;
        public float y;
        public float z;
        public float w;

        public quat(float w, vec3 vec)
        {
            this.x = vec.x;
            this.y = vec.y;
            this.z = vec.z;
            this.w = w;
        }
        public quat(vec3 v, float w)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = w;
        }
        public quat(quat other)
        {
            this.vec = other.vec;
            this.w = other.w;
        }

        public vec3 vec
        {
            get
            {
                return new vec3(x, y, z);
            }
            set
            {
                this.x = value.x;
                this.y = value.y;
                this.z = value.z;
            }
        }

        public quat Conjugate
        {
            get
            {
                return new quat(w, -vec);
            }
        }

        public quat AllNegative
        {
            get
            {
                return new quat(-w, -vec);
            }
        }

        public override string ToString()
        {
            return string.Format("({0}, {1}, {2}, {3}))", x, y, z, w);
        }

        public static quat operator *(quat l, quat r)
        {
            return new quat(r.vec * l.w + l.vec * r.w + vec3.cross(l.vec, r.vec),
                l.w * r.w - vec3.dot(l.vec, r.vec));
        }
        public static quat operator -(quat me)
        {
            return me.Conjugate;
        }
        public static quat Combine(quat current, quat extra)
        {
            return extra * current;
        }

        public vec3 In
        {
            get
            {
                return vec3.RotateAroundOrigo(this, vec3.In);
            }
        }
        public vec3 Right
        {
            get
            {
                return vec3.RotateAroundOrigo(this, vec3.Right);
            }
        }
        public vec3 Up
        {
            get
            {
                return vec3.RotateAroundOrigo(this, vec3.Up);
            }
        }

        public vec3 Out
        {
            get
            {
                return vec3.RotateAroundOrigo(this, vec3.Out);
            }
        }
        public vec3 Left
        {
            get
            {
                return vec3.RotateAroundOrigo(this, vec3.Left);
            }
        }
        public vec3 Down
        {
            get
            {
                return vec3.RotateAroundOrigo(this, vec3.Down);
            }
        }

        public static quat Identity
        {
            get
            {
                return new quat(1, vec3.Zero);
            }
        }

        public float Length
        {
            get
            {
                return (float)Math.Sqrt(LengthSquared);
            }
        }

        public float LengthSquared
        {
            get
            {
                return x * x + y * y + z * z + w*w;
            }
        }

        public static quat operator- (quat l, quat r)
        {
            return new quat(l.w - r.w, l.vec - r.vec);
        }
        public static quat operator +(quat l, quat r)
        {
            return new quat(l.w + r.w, l.vec + r.vec);
        }

        public static quat operator *(quat l, float r)
        {
            return new quat(l.w * r, l.vec * r);
        }
        public static quat operator *(float r, quat l)
        {
            return new quat(l.w * r, l.vec * r);
        }

        public static quat Slerp(quat a, float v, quat b)
        {
            float d = dot(a, b);
            if (d > 0.9995f)
            {
                return lerp(a, v, b);
            }
            d = math1.Within(-1, d, 1);
            angle theta0 = angle.Acos(d);
            angle theta = theta0 * v;

            quat q = (b - a * d).Normalized;
            return a * theta.Cos + q * theta.Sin;/*
            float cosOmega = a.x * b.x +
                                a.y * b.y +
                                a.z * b.z +
                                a.w * b.w;

            float sign = (cosOmega < 0.0f) ? (-1.0f) : (1.0f);
            cosOmega *= sign;

            float scaleFrom;
            float scaleTo;
            const float DELTA = 0.0001f;
            if ((1.0f - cosOmega) > DELTA)
            {
                // regular slerp
                angle omega = angle.Acos(cosOmega);
                float sinOmega = omega.Sin;
                scaleFrom = (float)Math.Sin((1.0f - v) * omega.inRadians) / sinOmega;
                scaleTo = (float)Math.Sin(v * omega.inRadians) / sinOmega;
            }
            else
            {
                // from and to are close, do a linear interpolation to speed things up a little
                scaleFrom = 1.0f - v;
                scaleTo = v;
            }
            scaleTo *= sign;

            return new quat(
                    scaleFrom * a.w + scaleTo * b.w, new vec3(
                    scaleFrom * a.x + scaleTo * b.x,
                    scaleFrom * a.y + scaleTo * b.y,
                    scaleFrom * a.z + scaleTo * b.z));*/
	    }

        private static quat lerp(quat a, float v, quat b)
        {
            return (a + v * (b - a)).Normalized;
        }

        public static float dot(quat A, quat B)
        {
            return A.x * B.x + A.y * B.y + A.z * B.z +A.w * B.w;
        }

	    // forces the interpolatation to go the "short way"
	    public static quat SlerpShortway(quat a, float t, quat b)
	    {
		    if( dot(a, b) < 0 ) return Slerp(a.AllNegative,t, b);
		    else return Slerp(a,t, b);
	    }

        private static bool isZero(float f)
        {
            return Math.Abs(f) < 0.006;
        }

        public AxisAngle AxisAngle
        {
            get
            {
                if  (isZero(x) && isZero(y) && isZero(z))
                {
                    return AxisAngle.RightHandAround(vec3.In, angle.FromRadians(0));
                }
                else
                {
                    return AxisAngle.RightHandAround(vec.Normalized, angle.Acos(w) * 2);
                }
            }
        }

        public quat(AxisAngle aa)
        {
            angle half = aa.angle * 0.5f;
            vec = aa.axis * half.Sin;
            w = half.Cos;
            normalize();
        }

        public void normalize()
        {
            float l = Length;
            x /= l;
            y /= l;
            z /= l;
            w /= l;
        }

        public quat Normalized
        {
            get
            {
                quat q = new quat(this);
                q.normalize();
                return q;
            }
        }

        public vec3 getRUI(vec3 rui)
        {
            return Right * rui.x + Up * rui.y + In * rui.z;
        }

        public mat33 mat33
        {
            get
            {
                float tXX = 2 * math1.Square(x);
                float tYY = 2 * math1.Square(y);
                float tZZ = 2 * math1.Square(z);
                float tXY = 2 * x * y;
                float tYZ = 2 * y * z;
                float tZW = 2 * z * w;
                float tXW = 2 * x * w;
                float tXZ = 2 * x * z;
                float tYW = 2 * y * w;
                return mat33.FromRowMajor(new float[] {
                    1-tYY-tZZ,  tXY-tZW,    tXZ+tYW,
                    tXY+tZW,    1-tXX-tZZ,  tYZ-tXW,
                    tXZ-tYW,    tYZ+tXW,    1-tXX-tYY
                });
            }
        }

        public float this[int index]
        {
            get
            {
                if (index == 0) return x;
                else if (index == 1) return y;
                else if (index == 2) return z;
                else if (index == 3) return w;
                else throw new Exception("bad index");
            }
            set
            {
                if (index == 0) x = value;
                else if (index == 1) y = value;
                else if (index == 2) z = value;
                else if (index == 3) w = value;
                else throw new Exception("bad index");
            }
        }

        public static quat FpsQuat(quat rotation, float dx, float dy)
        {
            quat rx = new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(-dx)));
            quat ry = new quat(AxisAngle.RightHandAround(rotation.Right, angle.FromDegrees(-dy)));
            quat final = rx * ry;
            return final;
        }

        internal static void Write(quat quat, System.IO.BinaryWriter bw)
        {
            bw.Write(quat.x);
            bw.Write(quat.y);
            bw.Write(quat.z);
            bw.Write(quat.w);
        }

        internal static quat Read(System.IO.BinaryReader br)
        {
            float x = br.ReadSingle();
            float y = br.ReadSingle();
            float z = br.ReadSingle();
            float w = br.ReadSingle();
            return new quat(w, new vec3(x, y, z));
        }
    }
}
