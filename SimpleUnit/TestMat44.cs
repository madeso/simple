using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using SimpleEngine;

namespace SimpleUnit
{
    [TestFixture]
    public class TestMat44 : Test
    {
        [Test]
        public void TestTransormation()
        {
            AreEqual(mat44.TranslationFor(new vec3(1, 2, 3)).Location, new vec3(1, 2, 3));
        }

        [Test]
        public void TestRight()
        {
            AreEqual(mat44.Identity.mat33.XAxis, vec3.Right);
        }

        [Test]
        public void TestUp()
        {
            AreEqual(mat44.Identity.mat33.YAxis, vec3.Up);
        }

        [Test]
        public void TestIn()
        {
            AreEqual(mat44.Identity.mat33.ZAxis, vec3.In);
        }

        [Test]
        public void TestRotation()
        {
            vec3 r = mat44.Identity.Help
                .Rotate(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(-90)))
                .transform(new vec3(0,0,5));
            AreEqual( r, new vec3(5, 0, 0));
        }

        [Test]
        public void TestCombined()
        {
            vec3 r = mat44.Identity.Help
                .Rotate(AxisAngle.RightHandAround(vec3.Up, angle.FromDegrees(-90)))
                .Translate(new vec3(0, 0, 5))
                .transform(new vec3(0, 0, 0));
            AreEqual(r, new vec3(5, 0, 0));
        }

        [Test]
        public void TestTranslation()
        {
            vec3 r = mat44.Identity.Help
                .Translate(new vec3(1,2,3))
                .transform(new vec3(7, 8, 9));
            AreEqual(r, new vec3(8, 10, 12));
        }

        [Test]
        public void TestIentityTransform()
        {
            vec3 r = mat44.Identity.Help
                .transform(new vec3(1, 2, 3));
            AreEqual(r, new vec3(1, 2, 3));
        }

        [Test]
        public void TestIentityMultiply()
        {
            AreEqual(mat44.Identity * mat44.Identity, mat44.Identity);
        }
    }
}
