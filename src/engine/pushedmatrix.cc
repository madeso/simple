using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    struct PushedMatrix : IDisposable
    {
        PushedMatrix()
        {
            Gl.glPushMatrix();
        }

        void Dispose()
        {
            Gl.glPopMatrix();
        }
    }
}
