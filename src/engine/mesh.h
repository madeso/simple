#pragma once

#include <memory>

#include "engine/compiledmesh.h"
#include "engine/media.h"

namespace simple
{

    /// @addtogroup anim
    /// @{

    /// A Media wrapper on a CompiledMesh
    struct Mesh : public Media
    {
        std::shared_ptr<CompiledMesh> mesh;

        void Load(MediaLoader* ml, FileSystem* fs, const std::string& path) override;
    };

    /// @}
}
