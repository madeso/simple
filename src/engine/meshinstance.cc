#include "engine/meshinstance.h"

#include "engine/compiledmesh.h"

namespace simple
{
    MeshInstance::MeshInstance(std::shared_ptr<Mesh> m)
        : mesh(m)
    {
    }

    void MeshInstance::SendToRenderer(RenderList* list)
    {
        mesh->mesh->SendToRenderer(list, position, rotation);
    }

    void MeshInstance::OnRender()
    {
    }
}
