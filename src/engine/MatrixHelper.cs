using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class MatrixHelper
    {
        mat44 mat;

        public MatrixHelper(mat44 mat)
        {
            this.mat = mat;
        }

        public MatrixHelper mult(mat44 m)
        {
            mat = mat * m;
            return this;
        }

        public MatrixHelper Rotate(AxisAngle aa)
        {
            return mult(mat44.FromAxisAngle(aa));
        }
        public MatrixHelper Rotate(quat q)
        {
            return mult(q.Conjugate.mat33.mat44);
        }

        public MatrixHelper Translate(vec3 t)
        {
            return mult(mat44.TranslationFor(t));
        }

        public vec3 transform(vec3 v)
        {
            return mat * v;
        }

        public mat33 mat33
        {
            get
            {
                return mat.mat33;
            }
        }

        public mat44 mat44
        {
            get
            {
                return mat;
            }
        }
    }
}
