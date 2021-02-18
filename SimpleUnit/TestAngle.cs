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
    public class TestAngle
    {
        private const float fe = 0.0000001f;
        [Test]
        public void TestSinCos()
        {
            Assert.That(angle.FromDegrees(90).Sin, Is.EqualTo(1).Within(fe));
            Assert.That(angle.FromDegrees(90).Cos, Is.EqualTo(0).Within(fe));
            Assert.That(angle.FromDegrees(0).Sin, Is.EqualTo(0).Within(fe));
            Assert.That(angle.FromDegrees(0).Cos, Is.EqualTo(1).Within(fe));
        }

        [Test]
        public void TestA()
        {
            Assert.That(angle.Asin(1).inDegrees, Is.EqualTo(90).Within(fe));
            Assert.That(angle.Acos(0).inDegrees, Is.EqualTo(90).Within(fe));
            Assert.That(angle.Asin(0).inDegrees, Is.EqualTo(0).Within(fe));
            Assert.That(angle.Acos(1).inDegrees, Is.EqualTo(0).Within(fe));
        }
    }
}
