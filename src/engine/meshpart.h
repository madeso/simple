#pragma once

#include <memory>
#include <vector>

#include "engine/material.h"
#include "engine/meshdef.h"
#include "engine/poseable.h"

namespace simple
{
    struct MediaLoader;
    struct MaterialDefinition;
    struct MeshDef;
    struct vec3;
    struct quat;

    /// @addtogroup anim
    /// @{

    struct MeshPart
    {
        std::vector<std::vector<Vertex>> faces;
        std::shared_ptr<Material> material;
        Poseable* poseable;
        int id = -1;

        MeshPart(MediaLoader* ml, const MaterialDefinition& m, MeshDef* def, Poseable* p);

        int GetId();

        int CalculateId() const;

        void OnRender(const vec3& position, const quat& rotation) const;
    };

    /// @}
}
