#include "engine/mesh.h"

#include <memory>

#include "engine/compiledmesh.h"
#include "engine/media.h"
#include "engine/meshfile.h"

namespace simple
{
    void Mesh::Load(MediaLoader* ml, FileSystem* fs, const std::string& path)
    {
        auto mesh = mesh_file::Load(fs, path);
        mesh->Compile(ml);
        this->mesh = mesh->GetCompiledMesh();
    }
}
