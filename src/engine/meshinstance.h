#pragma once

#include <memory>

#include "engine/mesh.h"
#include "engine/quat.h"
#include "engine/renderable.h"
#include "engine/vec3.h"

namespace simple
{
    struct MeshInstance : Renderable
    {
        std::shared_ptr<Mesh> mesh;
        vec3 position = vec3::Zero();
        quat rotation = quat::Identity();

        MeshInstance(std::shared_ptr<Mesh> m);

        void SendToRenderer(RenderList* list) override;

        void OnRender() override;
    };
}
