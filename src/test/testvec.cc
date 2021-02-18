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
    public class TestVec : Test
    {
        [Test]
        public void testX()
        {
            AreEqual(new vec3(1, 0, 0), new vec3(1, 0, 0));
            AreEqual(new vec3(2, 0, 0), new vec3(1, 0, 0) * 2);
            AreEqual(new vec3(2, 0, 0), new vec3(1, 0, 0) + new vec3(1, 0, 0));
        }

        [Test]
        public void testY()
        {
            AreEqual(new vec3(0, 1, 0), new vec3(0, 1, 0));
            AreEqual(new vec3(0, 2, 0), new vec3(0, 1, 0) * 2);
            AreEqual(new vec3(0, 2, 0), new vec3(0, 1, 0) + new vec3(0, 1, 0));
        }

        [Test]
        public void testZ()
        {
            AreEqual(new vec3(0, 0, 1), new vec3(0, 0, 1));
            AreEqual(new vec3(0, 0, 2), new vec3(0, 0, 1) * 2);
            AreEqual(new vec3(0, 0, 2), new vec3(0, 0, 1) + new vec3(0, 0, 1));
        }

        [Test]
        public void testDot()
        {
            Assert.AreEqual(0, vec3.dot(new vec3(1, 0, 0), new vec3(0, 1, 0)));
            Assert.AreEqual(1, vec3.dot(new vec3(1, 0, 0), new vec3(1, 0, 0)));
        }

        [Test]
        public void testCross()
        {
            AreEqual(new vec3(0, 1, 0), vec3.cross(new vec3(0, 0, -1), new vec3(-1, 0, 0)));
        }

        [Test]
        public void testInterpolate()
        {
            AreEqual(new vec3(0, 1, 0), vec3.Interpolate(new vec3(0, 1, 0), 0, new vec3(2, 4, 6)));
            AreEqual(new vec3(2, 4, 6), vec3.Interpolate(new vec3(0, 1, 0), 1, new vec3(2, 4, 6)));
            AreEqual(new vec3(3, 5, 7), vec3.Interpolate(new vec3(2, 4, 6), 0.5f, new vec3(4, 6, 8)));
        }
    }
}
