#include "engine/mesh.h"

#include <memory>

#include "engine/compiledmesh.h"
#include "engine/media.h"
#include "engine/meshfile.h"

namespace SimpleEngine
{
    void Mesh::load(MediaLoader* ml, FileSystem* fs, const std::string& path)
    {
        auto mesh = MeshFile::Load(fs, path);
        mesh->compile(ml);
        this->mesh = mesh->Compiled();
    }

    std::shared_ptr<CompiledMesh> Mesh::Compiled()
    {
        return mesh;
    }
}
