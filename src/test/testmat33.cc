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
    public class TestMat33 : Test
    {
        private const float fe = 0.001f;
        [Test]
        public void TestColumnMajor()
        {
            mat33 m = mat33.FromColumnMajor(new float[] { 0,1,2,3,4,5,6,7,8 });
            Assert.That(m[0, 0], Is.EqualTo(0).Within(fe));
            Assert.That(m[1, 0], Is.EqualTo(1).Within(fe));
            Assert.That(m[2, 0], Is.EqualTo(2).Within(fe));

            Assert.That(m[0, 1], Is.EqualTo(3).Within(fe));
            Assert.That(m[1, 1], Is.EqualTo(4).Within(fe));
            Assert.That(m[2, 1], Is.EqualTo(5).Within(fe));

            Assert.That(m[0, 2], Is.EqualTo(6).Within(fe));
            Assert.That(m[1, 2], Is.EqualTo(7).Within(fe));
            Assert.That(m[2, 2], Is.EqualTo(8).Within(fe));
        }
        /*[Test]
        public void TestTransposed()
        {
            mat33 m = mat33.FromColumnMajor(new float[] { 0, 1, 2, 3, 4, 5, 6, 7, 8 }).Transposed;
            Assert.That(m[0, 0], Is.EqualTo(0).Within(fe));
            Assert.That(m[0, 1], Is.EqualTo(1).Within(fe));
            Assert.That(m[0, 2], Is.EqualTo(2).Within(fe));

            Assert.That(m[1, 0], Is.EqualTo(3).Within(fe));
            Assert.That(m[1, 1], Is.EqualTo(4).Within(fe));
            Assert.That(m[1, 2], Is.EqualTo(5).Within(fe));

            Assert.That(m[2, 0], Is.EqualTo(6).Within(fe));
            Assert.That(m[2, 1], Is.EqualTo(7).Within(fe));
            Assert.That(m[2, 2], Is.EqualTo(8).Within(fe));
        }*/

        [Test]
        public void TestRowMajor()
        {
            mat33 m = mat33.FromRowMajor(new float[] { 0, 1, 2, 3, 4, 5, 6, 7, 8 });
            Assert.That(m[0, 0], Is.EqualTo(0).Within(fe));
            Assert.That(m[0, 1], Is.EqualTo(1).Within(fe));
            Assert.That(m[0, 2], Is.EqualTo(2).Within(fe));

            Assert.That(m[1, 0], Is.EqualTo(3).Within(fe));
            Assert.That(m[1, 1], Is.EqualTo(4).Within(fe));
            Assert.That(m[1, 2], Is.EqualTo(5).Within(fe));

            Assert.That(m[2, 0], Is.EqualTo(6).Within(fe));
            Assert.That(m[2, 1], Is.EqualTo(7).Within(fe));
            Assert.That(m[2, 2], Is.EqualTo(8).Within(fe));
        }

        [Test]
        public void TestFromToMat44()
        {
            AreEqual(mat33.Identity.mat44.mat33, mat33.Identity);
        }
    }
}
