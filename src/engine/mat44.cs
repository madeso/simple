using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class mat44
    {
        const int kSize = 4;

        /*
         *     | 0 4 8  12 |
         * M = | 1 5 9  13 |
         *     | 2 5 10 14 |
         *     | 3 7 11 15 |
         */

        private float[] dataColumnMajor;
        public float[] DataArray
        {
            get
            {
                return dataColumnMajor;
            }
        }

        public float this[int row, int column]
        {
            get
            {
                return dataColumnMajor[row + column * kSize];
            }
            set
            {
                dataColumnMajor[row + column * kSize] = value;
            }
        }

        private mat44(float[] dataColumnMajor)
        {
            if(dataColumnMajor.Length != math1.Square(kSize) ) throw new Exception("size is " + dataColumnMajor.Length);
            this.dataColumnMajor = dataColumnMajor;
        }

        public static mat44 FromColumnMajor(float[] data)
        {
            return new mat44(data);
        }

        public static mat44 FromRowMajor(float[] data)
        {
            return new mat44(new float[] {
                data[0], data[4], data[8],  data[12],
                data[1], data[5], data[9],  data[13],
                data[2], data[6], data[10], data[14],
                data[3], data[7], data[11], data[15]
            } );
        }

        public static mat44 operator* (mat44 a, mat44 b)
        {
            return mat44.FromRowMajor(new float[] {
                multsum(a,b, 0,0), multsum(a, b, 0, 1), multsum(a, b, 0, 2), multsum(a, b, 0, 3),
                multsum(a,b, 1,0), multsum(a, b, 1, 1), multsum(a, b, 1, 2), multsum(a, b, 1, 3),
                multsum(a,b, 2,0), multsum(a, b, 2, 1), multsum(a, b, 2, 2), multsum(a, b, 2, 3),
                multsum(a,b, 3,0), multsum(a, b, 3, 1), multsum(a, b, 3, 2), multsum(a, b, 3, 3),
            });
        }
        public static vec3 operator *(mat44 m, vec3 v)
        {
            return new vec3(multsum(m, v, 0), multsum(m, v, 1), multsum(m, v, 2));
        }

        private static float multsum(mat44 a, mat44 b, int row, int col)
        {
            return a[row, 0] * b[0, col] + a[row, 1] * b[1, col] + a[row, 2] * b[2, col] + a[row, 3] * b[3, col];
        }
        private static float multsum(mat44 a, vec3 b, int row)
        {
            return a[row, 0] * b[0] + a[row, 1] * b[1] + a[row, 2] * b[2]+ a[row, 3] * 1;
        }

        public static mat44 TranslationFor(vec3 v)
        {
            return FromRowMajor(new float[] {
                1,0,0,v.x,
                0,1,0,v.y,
                0,0,1,v.z,
                0,0,0,1
            });
        }

        public mat33 mat33
        {
            get
            {
                return mat33.FromRowMajor(new float[] {
                    this[0, 0], this[0, 1], this[0, 2],
                    this[1, 0], this[1, 1], this[1, 2],
                    this[2, 0], this[2, 1], this[2, 2]
                });
            }
        }

        public vec3 Location
        {
            get
            {
                return new vec3(this[0, 3], this[1, 3], this[2, 3]);
            }
            /* is this mathematically correct?
            set
            {
                this[0, 3] = value.x;
                this[1, 3] = value.y;
                this[2, 3] = value.z;
            }*/
        }

        public MatrixHelper Help
        {
            get
            {
                return new MatrixHelper(this);
            }
        }

        /*public mat44 Transposed
        {
            get
            {
                return FromRowMajor(dataColumnMajor);
            }
        }*/

        public override string ToString()
        {
            return string.Format("({0} {1} {2} {3})", rowAsString(0), rowAsString(1), rowAsString(2), rowAsString(3));
        }

        private string rowAsString(int row)
        {
            return string.Format("({0} {1} {2} {3})", nice(this[row, 0]), nice(this[row, 1]), nice(this[row, 2]), nice(this[row, 3]));
        }

        private float nice(float p)
        {
            if (math1.IsZero(p)) return 0;
            else return p;
        }

        public static mat44 FromAxisAngle(AxisAngle aa)
        {
            float c = aa.angle.Cos;
            float s = aa.angle.Sin;
            float x = aa.axis.x;
            float y = aa.axis.y;
            float z = aa.axis.z;

            return new mat44(new float[] {
    	      x*x*(1-c)+c,      x*y*(1-c)-z*s,  x*z*(1-c)+y*s,  0,
		      y*x*(1-c)+z*s,    y*y*(1-c)+c,    y*z*(1-c)-x*s,  0,
		      x*z*(1-c)-y*s,    y*z*(1-c)+x*s,  z*z*(1-c)+c,    0,
		      0,                0,              0,              1
            });
        }

        public static mat44 Identity
        {
            get
            {
                return new mat44(new float[] {
                    1,0,0,0,
                    0,1,0,0,
                    0,0,1,0,
                    0,0,0,1
                });
            }
        }
    }
}
