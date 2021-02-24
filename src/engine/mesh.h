﻿#pragma once

#include <memory>

#include "engine/compiledmesh.h"
#include "engine/media.h"

namespace SimpleEngine
{
    struct Mesh : public Media
    {
        std::shared_ptr<CompiledMesh> mesh;

        void load(MediaLoader* ml, FileSystem* fs, const std::string& path) override;

        std::shared_ptr<CompiledMesh> Compiled();
    };
}