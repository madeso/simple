using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    public class Material
    {
        public Material(vec3 color, Texture tex)
        {
            this.color = color;
            this.tex = new Handle<Texture>(tex);
        }

        public void apply()
        {
            Gl.glColor3f(color.x, color.y, color.z);
            tex.Ref.bind();
        }

        vec3 color = new vec3(1,1,1);
        Handle<Texture> tex;
    }
}
