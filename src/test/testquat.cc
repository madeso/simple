using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using NUnit.Framework;

using SimpleEngine;
using NUnit.Framework.SyntaxHelpers;

namespace SimpleUnit
{
    [TestFixture]
    public class TestQuat : Test
    {
        [Test]
        public void TestCommonRotations()
        {
            AreEqual(MakeQuat(0, 1, 0, 0), new quat(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(180))));
	        AreEqual(MakeQuat(0, 0, 1, 0), new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(180))));
	        AreEqual(MakeQuat(0, 0, 0, 1), new quat(AxisAngle.RightHandAround(vec3.Out, angle.FromDegrees(180))));

	        float k = (float) Math.Sqrt(0.5);
	        AreEqual(MakeQuat(k, k, 0, 0), new quat(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(90))));
	        AreEqual(MakeQuat(k, 0, k, 0), new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(90))));
	        AreEqual(MakeQuat(k, 0, 0, k), new quat(AxisAngle.RightHandAround(vec3.Out, angle.FromDegrees(90))));

	        AreEqual(MakeQuat(k, -k, 0, 0), new quat(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(-90))));
	        AreEqual(MakeQuat(k, 0, -k, 0), new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(-90))));
	        AreEqual(MakeQuat(k, 0, 0, -k), new quat(AxisAngle.RightHandAround(vec3.Out, angle.FromDegrees(-90))));

	        AreEqual(MakeQuat(k, k, 0, 0), new quat(AxisAngle.RightHandAround(vec3.Left, angle.FromDegrees(-90))));
	        AreEqual(MakeQuat(k, 0, k, 0), new quat(AxisAngle.RightHandAround(vec3.Down, angle.FromDegrees(-90))));
	        AreEqual(MakeQuat(k, 0, 0, k), new quat(AxisAngle.RightHandAround(vec3.In, angle.FromDegrees(-90))));

	        AreEqual(MakeQuat(k, -k, 0, 0), new quat(AxisAngle.RightHandAround(vec3.Left, angle.FromDegrees(90))));
	        AreEqual(MakeQuat(k, 0, -k, 0), new quat(AxisAngle.RightHandAround(vec3.Down, angle.FromDegrees(90))));
	        AreEqual(MakeQuat(k, 0, 0, -k), new quat(AxisAngle.RightHandAround(vec3.In, angle.FromDegrees(90))));
        }

        private static quat MakeQuat(float w, float x, float y, float z)
        {
            return new quat(w, new vec3(x, y, z) );
        }

        [Test]
        public void testMat33()
        {
            AreEqual(quat.Identity.mat33.quat, quat.Identity);
        }

        [Test]
        public void testConstants()
        {
	        AreEqual(MakeQuat(1, 0, 0, 0), quat.Identity);
            Assert.AreEqual(0, quat.Identity.x);
            Assert.AreEqual(0, quat.Identity.y);
            Assert.AreEqual(0, quat.Identity.z);
            Assert.AreEqual(1, quat.Identity.w);
        }

	    /*[Test]
        public void testLook()
	    {
		    AreEqual(new quat(AxisAngle.RightHandAround(Up(), angle.FromDegrees(-90))),  quat.LookAt(vec3(0,0,0), vec3(5,0,0), Up );
		    AreEqual(new quat(AxisAngle.RightHandAround(Up(), angle.FromDegrees(-90))),  quat.LookAt(vec3(0,0,0), vec3(2,0,0), Up );
		    AreEqual(new quat(AxisAngle.RightHandAround(Up(), angle.FromDegrees(-90))),  quat.LookAt(vec3(0,0,0), vec3(0.25f,0,0), Up );

		    AreEqual(new quat(AxisAngle.RightHandAround(Up(), angle.FromDegrees(180))),  quat.LookInDirection(vec3(0,0,9), Up );
		    AreEqual(new quat(AxisAngle.RightHandAround(Up(), angle.FromDegrees(-90))),  quat.LookInDirection(vec3(3,0,0), Up );
		    AreEqual(new quat(AxisAngle.RightHandAround(Up(), angle.FromDegrees(90))),  quat.LookInDirection(vec3(-5,0,0), Up );

		    // todo: add more test where up != Up()
	    }*/

	    [Test]
        public void testCombine()
	    {
            AreEqual(new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(90))),
                 quat.Combine(new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(90))), quat.Identity));

            AreEqual(new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(90))),
                 quat.Combine(quat.Identity, new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(90)))));

		    AreEqual( quat.Combine(new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(-90))),
			                           new quat(AxisAngle.RightHandAround(vec3.Out, angle.FromDegrees(90)))),
                 quat.Combine(new quat(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(90))),
                               new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(-90)))));
	    }

        [Test]
        public void testLocalAxis()
        {
            quat q = quat.Identity;

            AreEqual(vec3.Right, q.Right);
            AreEqual(vec3.Up, q.Up);
            AreEqual(vec3.In, q.In);

            q = new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(90)));
            AreEqual(vec3.In, q.Right);
            AreEqual(vec3.Up, q.Up);
            AreEqual(vec3.Left, q.In);

            q = new quat(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(90)));
            AreEqual(vec3.Right, q.Right);
            AreEqual(vec3.Out, q.Up);
            AreEqual(vec3.Up, q.In);

            q = new quat(AxisAngle.RightHandAround(vec3.In, angle.FromDegrees(90)));
            AreEqual(vec3.Down, q.Right);
            AreEqual(vec3.Right, q.Up);
            AreEqual(vec3.In, q.In);
        }

        [Test]
        public void testVecOp()
        {
            AreEqual(new vec3(1, 2, 3), new quat(4, new vec3(1, 2, 3)).vec);
        }

        [Test]
        public void testMovement()
        {
            quat q = quat.Identity;
            //AreEqual(q.In, new vec3(0, 0, 
        }

        // failing...

	    [Test]
        public void verifyTestAxisAngle()
	    {
            AreEqual(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(0)), AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(0)));
            AreEqual(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(90)), AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(90)));
            AreEqual(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(-45)), AxisAngle.RightHandAround(-vec3.Up, angle.FromDegrees(45)));
            AreEqual(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(90)), AxisAngle.RightHandAround(-vec3.Right, angle.FromDegrees(-90)));
	    }

	    [Test]
        public void checkAxisAngle()
	    {
            //AreEqual(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(0)), new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(0))).AxisAngle);
            //AreEqual(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(0)), new quat(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(0))).AxisAngle);
            AreEqual(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(90)), new quat(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(90))).AxisAngle);
            AreEqual(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(-45)), new quat(AxisAngle.RightHandAround(-vec3.Up, angle.FromDegrees(45))).AxisAngle);
            AreEqual(AxisAngle.RightHandAround(vec3.Right, angle.FromDegrees(90)), new quat(AxisAngle.RightHandAround(-vec3.Right, angle.FromDegrees(-90))).AxisAngle);
	    }

        quat qa = new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(45)));
        quat qb = new quat(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(90)));

        [Test]
        public void testSlerp1()
        {
            AreEqual(quat.Identity, quat.Slerp(quat.Identity, 0, qb));
        }
        [Test]
        public void testSlerp2()
        {
            AreEqual(qb, quat.Slerp(quat.Identity, 1, qb));
        }
        [Test]
        public void testSlerp3()
        {
            AreEqual(quat.Identity, quat.SlerpShortway(quat.Identity, 0, qb));
        }
        [Test]
        public void testSlerp4()
        {
            AreEqual(qb, quat.SlerpShortway(quat.Identity, 1, qb));
        }
        [Test]
        public void testSlerp5()
        {
            AreEqual(qa, quat.SlerpShortway(quat.Identity, 0.5f, qb));
        }
    }
}
