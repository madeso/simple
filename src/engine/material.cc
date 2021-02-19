using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    struct Material
    {
        Material(vec3 color, Texture tex)
        {
            this.color = color;
            this.tex = Handle<Texture>(tex);
        }

        void apply()
        {
            Gl.glColor3f(color.x, color.y, color.z);
            tex.Ref.bind();
        }

        vec3 color = vec3(1, 1, 1);
        Handle<Texture> tex;
    }
}
