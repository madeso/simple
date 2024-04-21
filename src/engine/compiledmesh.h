#pragma once

#include <memory>

#include "engine/animation.h"
#include "engine/meshpart.h"
#include "engine/poseable.h"

namespace simple
{
    struct MediaLoader;
    struct RenderList;
    struct vec3;
    struct quat;

    /// @addtogroup anim
    /// @{

    /// A list of MeshPart with a CompiledPose
    struct CompiledMesh : public Poseable
    {
        std::vector<std::shared_ptr<MeshPart>> parts;
        std::shared_ptr<CompiledPose> pose;

        CompiledMesh(MediaLoader* ml, MeshDef* def);

        void SendToRenderer(RenderList* r, const vec3& pos, const quat& rot);

        void SetPose(std::shared_ptr<CompiledPose> new_pose);

        std::shared_ptr<CompiledPose> GetPose() override;
    };

    /// @}
}
