using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    public class FullscreenQuad
    {
        public static void render(Fbo texture, int Width, int Height)
        {
            Setup.view2d(Width, Height);

            if (texture != null)
            {
                texture.bindTexture();
            }

            Gl.glBegin(Gl.GL_QUADS);
            Gl.glTexCoord2f(0, 1); Gl.glVertex2f(0, 0); // top left
            Gl.glTexCoord2f(0, 0); Gl.glVertex2f(0, Height); // bottom left
            Gl.glTexCoord2f(1, 0); Gl.glVertex2f(Width, Height); // bottom right
            Gl.glTexCoord2f(1, 1); Gl.glVertex2f(Width, 0); // top right
            Gl.glEnd();
        }
    }
}
