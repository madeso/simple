using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SimpleEngine;
using NUnit.Framework;
using NUnit.Framework.SyntaxHelpers;
using NUnit.Framework.Constraints;

namespace SimpleUnit
{
    public class Test
    {
        const float fs = 0.001f;

        private static bool IsClose(float a, float b)
        {
            return Math.Abs(a - b) < fs;
        }
        private static bool IsClose(quat a, quat b)
        {
            return IsClose(a.x, b.x) && IsClose(a.y, b.y) && IsClose(a.z, b.z) && IsClose(a.w, b.w);
        }
        private static bool IsClose(vec3 a, vec3 b)
        {
            return IsClose(a.x, b.x) && IsClose(a.y, b.y) && IsClose(a.z, b.z);
        }
        private static bool IsClose(vec2 a, vec2 b)
        {
            return IsClose(a.x, b.x) && IsClose(a.y, b.y);
        }
        private static bool IsClose(AxisAngle a, AxisAngle b)
        {
            return IsClose(a.axis, b.axis) && IsClose(a.angle.inDegrees, b.angle.inDegrees);
        }
        private static bool IsClose(mat33 a, mat33 b)
        {
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    if (IsClose(a[i, j], b[i, j]) == false) return false;
                }
            }
            return true;
        }

        private static bool IsClose(mat44 a, mat44 b)
        {
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    if (IsClose(a[i, j], b[i, j]) == false) return false;
                }
            }
            return true;
        }

        private class QuatContr : Constraint
        {
            quat q;
            quat act;

            public QuatContr(quat q)
            {
                this.q = q;
            }
            public override bool Matches(object actual)
            {
                act = (quat)actual;
                return IsClose((quat)actual, q);
            }

            public override void WriteDescriptionTo(MessageWriter writer)
            {
                writer.Write(q);
            }

            public override void WriteActualValueTo(MessageWriter writer)
            {
                writer.Write(act);
            }
        }

        private class AxisAngConstr : Constraint
        {
            AxisAngle a;
            AxisAngle act;

            public AxisAngConstr(AxisAngle a)
            {
                this.a = a;
            }
            public override bool Matches(object actual)
            {
                act = (AxisAngle)actual;
                if( IsClose(act.angle.inDegrees, 0) && IsClose(a.angle.inDegrees, 0) ) return true;
                return IsClose(act, a) || IsClose(-act, a);
            }

            public override void WriteDescriptionTo(MessageWriter writer)
            {
                writer.Write( a.ToString() );
            }

            public override void WriteActualValueTo(MessageWriter writer)
            {
                writer.Write(act.ToString());
            }
        }

        private class mat33Contraint : Constraint
        {
            mat33 a;
            mat33 act;

            public mat33Contraint(mat33 a)
            {
                this.a = a;
            }
            public override bool Matches(object actual)
            {
                act = (mat33)actual;
                return IsClose(act, a);
            }

            public override void WriteDescriptionTo(MessageWriter writer)
            {
                writer.Write(a.ToString());
            }

            public override void WriteActualValueTo(MessageWriter writer)
            {
                writer.Write(act.ToString());
            }
        }
        private class mat44Contraint : Constraint
        {
            mat44 a;
            mat44 act;

            public mat44Contraint(mat44 a)
            {
                this.a = a;
            }
            public override bool Matches(object actual)
            {
                act = (mat44)actual;
                return IsClose(act, a);
            }

            public override void WriteDescriptionTo(MessageWriter writer)
            {
                writer.Write(a.ToString());
            }

            public override void WriteActualValueTo(MessageWriter writer)
            {
                writer.Write(act.ToString());
            }
        }

        public void AreEqual(quat a, quat b)
        {
            Assert.That(a, new QuatContr(b));
        }
        public void AreEqual(vec3 a, vec3 b)
        {
            string msg = "{0} != {1}";
            Assert.That(a.x, Is.EqualTo(b.x).Within(fs), msg, a, b);
            Assert.That(a.y, Is.EqualTo(b.y).Within(fs), msg, a, b);
            Assert.That(a.z, Is.EqualTo(b.z).Within(fs), msg, a, b);
        }
        public void AreEqual(AxisAngle a, AxisAngle b)
        {
            Assert.That(a, new AxisAngConstr(b));
        }
        public void AreEqual(mat33 a, mat33 b)
        {
            Assert.That(a, new mat33Contraint(b));
        }
        public void AreEqual(mat44 a, mat44 b)
        {
            Assert.That(a, new mat44Contraint(b));
        }
    }
}
