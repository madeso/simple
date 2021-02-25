#include "engine/meshinstance.h"

#include "engine/compiledmesh.h"

namespace SimpleEngine
{
    MeshInstance::MeshInstance(std::shared_ptr<Mesh> def)
        : mesh(def)
    {
    }

    void MeshInstance::SendToRenderer(RenderList* r)
    {
        mesh->mesh->SendToRenderer(r, pos, rot);
    }

    void MeshInstance::OnRender()
    {
    }
}
