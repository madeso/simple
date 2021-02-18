using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    class PushedMatrix :  IDisposable
    {
        public PushedMatrix()
        {
            Gl.glPushMatrix();
        }

        public void Dispose()
        {
            Gl.glPopMatrix();
        }
    }
}
