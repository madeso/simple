using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    // matrix order:3x3
    public class mat33
    {
        const int kSize = 3;

        /*
         *     | 0 3 6 |
         * M = | 1 4 7 |
         *     | 2 5 8 |
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

        private mat33(float[] data)
        {
            System.Diagnostics.Debug.Assert(data.Length == math1.Square(kSize));
            this.dataColumnMajor = data;
        }

        public static mat33 FromColumnMajor(float[] data)
        {
            return new mat33(data);
        }

        public static mat33 FromRowMajor(float[] data)
        {
            return new mat33( new float[] {
                data[0], data[3], data[6],
                data[1], data[4], data[7],
                data[2], data[5], data[8]
            } );
        }

        public vec3 XAxis
        {
            get
            {
                return new vec3(this[0, 0], this[1, 0], this[2, 0]);
            }
        }

        public vec3 YAxis
        {
            get
            {
                return new vec3(this[0, 1], this[1, 1], this[2, 1]);
            }
        }

        public vec3 ZAxis
        {
            get
            {
                return - new vec3(this[0, 2], this[1, 2], this[2, 2]);
            }
        }

        public mat44 mat44
        {
            get
            {
                return mat44.FromRowMajor(new float[] {
                    this[0, 0], this[0, 1], this[0, 2], 0,
                    this[1, 0], this[1, 1], this[1, 2], 0,
                    this[2, 0], this[2, 1], this[2, 2], 0,
                    0,0,0,1
                });
            }
        }

        /*public mat33 Transposed
        {
            get
            {
                return FromRowMajor(dataColumnMajor);
            }
        }*/


        public static mat33 Identity
        {
            get
            {
                return new mat33(new float[] {
                    1,0,0,
                    0,1,0,
                    0,0,1
                });
            }
        }

        public static mat33 Scale(vec3 scale)
        {
            return FromRowMajor(new float[] {
                scale.x, 0, 0,
                0, scale.y, 0,
                0, 0, scale.z
            });
        }

        public quat quat
		{
            get
            {
                // check the diagonal
                float tr = this[0, 0] + this[1, 1] + this[2, 2];
                if (tr > 0.0f)
                {
                    float s = math1.Sqrt(tr + 1.0f);
                    float t = 0.5f / s;

                    return new quat(
                        s * 0.5f,
                        new vec3((this[1, 2] - this[2, 1]) * t,
                                 (this[2, 0] - this[0, 2]) * t,
                                 (this[0, 1] - this[1, 0]) * t));
                }
                else
                {
                    int[] NXT = new int[] { 1, 2, 0 };
                    quat q = quat.Identity;
                    // diagonal is negative
                    // get biggest diagonal element
                    int i = 0;
                    if (this[1, 1] > this[0, 0]) i = 1;
                    if (this[2, 2] > this[i, i]) i = 2;
                    //setup index sequence
                    int j = NXT[i];
                    int k = NXT[j];

                    float s = math1.Sqrt((this[i, i] - (this[j, j] + this[k, k])) + 1.0f);

                    q[i] = s * 0.5f;

                    
                    if (math1.IsZero(s)==false) s = 0.5f / s;

                    q[j] = (this[i, j] + this[j, i]) * s;
                    q[k] = (this[i, k] + this[k, i]) * s;
                    q[3] = (this[j, k] - this[k, j]) * s;

                    return q;
                }
            }
		}
    }
}
