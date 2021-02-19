#pragma once

#include <memory>
#include <vector>

#include "engine/material.h"
#include "engine/meshdef.h"
#include "engine/poseable.h"

namespace SimpleEngine
{
    struct MediaLoader;
    struct MaterialDef;
    struct MeshDef;
    struct vec3;
    struct quat;

    struct MeshPart
    {
        std::vector<std::vector<Vertex>> faces;
        std::shared_ptr<Material> mat;
        Poseable* poseable;
        int id = -1;

        MeshPart(MediaLoader* ml, const MaterialDef& m, MeshDef* def, Poseable* p);

        int Id();

        int calculateId() const;

        void render(const vec3& p, const quat& rot) const;
    };
}
