#include "engine/compiledmesh.h"

#include <memory>
#include <stdexcept>

#include "engine/renderlist.h"

namespace SimpleEngine
{
    CompiledMesh::CompiledMesh(MediaLoader* ml, MeshDef* def)
    {
        std::vector<std::string> errors;
        for (auto& m : def->Materials())
        {
            if (def->hasTrianglesFor(*m))
            {
                try
                {
                    parts.emplace_back(std::make_shared<MeshPart>(ml, *m, def, this));
                }
                catch (const std::runtime_error& e)
                {
                    errors.emplace_back(e.what());
                }
            }
        }

        if (errors.empty() == false)
        {
            throw std::runtime_error("Several errors occured");
        }
    }

    void CompiledMesh::sendToRenderer(RenderList* r, const vec3& pos, const quat& rot)
    {
        for (auto& part : parts)
        {
            r->add(part, pos, rot);
        }
    }

    void CompiledMesh::setPose(std::shared_ptr<CompiledPose> new_pose)
    {
        pose = new_pose;
    }

    std::shared_ptr<CompiledPose> CompiledMesh::CurrentPose()
    {
        return pose;
    }
}
