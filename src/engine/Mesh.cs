using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class Mesh : Media
    {
        CompiledMesh mesh = null;

        public override void load(MediaLoader ml, FileSystem fs, string path)
        {
            MeshDef mesh = MeshFile.Load(fs, path);
            mesh.compile(ml);
            this.mesh = mesh.Compiled;
        }

        protected override void unload()
        {
            mesh = null;
        }

        public CompiledMesh Compiled
        {
            get
            {
                return mesh;
            }
        }
    }
}
