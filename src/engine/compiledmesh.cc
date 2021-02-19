using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct Poseable
    {
        CompiledPose CurrentPose
        {
            get;
        }
    } struct CompiledMesh : Poseable
    {
        std::vector<MeshPart> parts = std::vector<MeshPart>();

        CompiledMesh(MediaLoader ml, MeshDef def)
        {
            std::vector<Exception> errors = std::vector<Exception>();
            for (MeshDef.MaterialDef m : def.Materials)
            {
                if (def.hasTrianglesFor(m))
                {
                    try
                    {
                        parts.Add(MeshPart(ml, m, def, this));
                    }
                    catch (Exception e)
                    {
                        errors.Add(e);
                    }
                }
            }

            if (errors.Count != 0)
            {
                throw Exception("Several errors occured");
            }
        }

        void sendToRenderer(RenderList r, vec3 pos, quat rot)
        {
            for (MeshPart part : parts)
            {
                r.add(part, pos, rot);
            }
        }

        CompiledPose pose = nullptr;

        void setPose(CompiledPose pose)
        {
            this.pose = pose;
        }

        override CompiledPose CurrentPose
        {
            get { return pose; }
        }
    }
}
