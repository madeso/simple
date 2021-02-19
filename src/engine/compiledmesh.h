#include <memory>

#include "engine/animation.h"
#include "engine/meshpart.h"
#include "engine/poseable.h"

namespace SimpleEngine
{
    struct MediaLoader;
    struct RenderList;
    struct vec3;
    struct quat;

    struct CompiledMesh : public Poseable
    {
        std::vector<std::shared_ptr<MeshPart>> parts;
        std::shared_ptr<CompiledPose> pose;

        CompiledMesh(MediaLoader* ml, MeshDef* def);

        void sendToRenderer(RenderList* r, const vec3& pos, const quat& rot);

        void setPose(std::shared_ptr<CompiledPose> new_pose);

        std::shared_ptr<CompiledPose> CurrentPose() override;
    };
}
