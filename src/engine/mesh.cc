using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct Mesh : Media
    {
        CompiledMesh mesh = nullptr;

        override void load(MediaLoader ml, FileSystem fs, std::string path)
        {
            MeshDef mesh = MeshFile.Load(fs, path);
            mesh.compile(ml);
            this.mesh = mesh.Compiled;
        }

    protected
        override void unload()
        {
            mesh = nullptr;
        }

        CompiledMesh Compiled
        {
            get
            {
                return mesh;
            }
        }
    }
}
