#include "engine/meshinstance.h"

#include "engine/compiledmesh.h"

namespace SimpleEngine
{
    MeshInstance::MeshInstance(std::shared_ptr<Mesh> def)
        : mesh(def)
    {
    }

    void MeshInstance::sendToRenderer(RenderList* r)
    {
        mesh->Compiled()->sendToRenderer(r, pos, rot);
    }

    void MeshInstance::render()
    {
    }
}
