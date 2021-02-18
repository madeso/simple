using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public abstract class Poseable
    {
        public abstract CompiledPose CurrentPose
        {
            get;
        }
    }
    public class CompiledMesh : Poseable
    {
        List<MeshPart> parts = new List<MeshPart>();

        public CompiledMesh(MediaLoader ml, MeshDef def)
        {
            List<Exception> errors = new List<Exception>();
            foreach (MeshDef.MaterialDef m in def.Materials)
            {
                if (def.hasTrianglesFor(m))
                {
                    try
                    {
                        parts.Add(new MeshPart(ml, m, def, this));
                    }
                    catch (Exception e)
                    {
                        errors.Add(e);
                    }
                }
            }

            if (errors.Count != 0)
            {
                throw new Exception("Several errors occured");
            }
        }

        public void sendToRenderer(RenderList r, vec3 pos, quat rot)
        {
            foreach (MeshPart part in parts)
            {
                r.add(part, pos, rot);
            }
        }

        CompiledPose pose = null;

        public void setPose(CompiledPose pose)
        {
            this.pose = pose;
        }

        public override CompiledPose CurrentPose
        {
            get { return pose; }
        }
    }
}
