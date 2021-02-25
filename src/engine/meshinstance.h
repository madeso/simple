#pragma once

#include <memory>

#include "engine/mesh.h"
#include "engine/quat.h"
#include "engine/renderable.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    struct MeshInstance : Renderable
    {
        std::shared_ptr<Mesh> mesh;
        vec3 pos = vec3::Zero();
        quat rot = quat::Identity();

        MeshInstance(std::shared_ptr<Mesh> def);

        void SendToRenderer(RenderList* r) override;

        void OnRender() override;
    };
}
