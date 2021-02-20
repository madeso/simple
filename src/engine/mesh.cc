#include <memory>

#include "engine/compiledmesh.h"
#include "engine/media.h"

namespace SimpleEngine
{
    struct Mesh : public Media
    {
        std::shared_ptr<CompiledMesh> mesh;

        void load(MediaLoader* ml, FileSystem* fs, const std::string& path) override
        {
            auto mesh = MeshFile.Load(fs, path);
            mesh.compile(ml);
            this->mesh = mesh.Compiled();
        }

        std::shared_ptr<CompiledMesh> Compiled()
        {
            return mesh;
        }
    }
}
