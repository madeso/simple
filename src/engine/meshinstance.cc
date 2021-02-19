using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct MeshInstance : Renderable
    {
        Handle<Mesh> mesh;
        vec3 pos = vec3::Zero();
        quat rot = quat::Identity();

        MeshInstance(Mesh def)
        {
            this.mesh = Handle<Mesh>(def);
        }

        void sendToRenderer(RenderList r)
        {
            mesh.Ref.Compiled.sendToRenderer(r, pos, rot);
        }

        void render()
        {
        }
    }
}
