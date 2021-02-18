using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class MeshInstance : Renderable
    {
        Handle<Mesh> mesh;
        public vec3 pos = vec3.Zero;
        public quat rot = quat.Identity;

        public MeshInstance(Mesh def)
        {
            this.mesh = new Handle<Mesh>(def);
        }

        public void sendToRenderer(RenderList r)
        {
            mesh.Ref.Compiled.sendToRenderer(r, pos, rot);
        }

        public void render()
        {
        }
    }
}
