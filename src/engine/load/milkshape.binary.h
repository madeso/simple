#pragma once

#include <memory>
#include <string>

namespace SimpleEngine
{
    struct FileSystem;
    struct MeshDef;
    struct Animation;
}

namespace SimpleEngine::load::MilkshapeBinary
{
    void Load(FileSystem* fs, const std::string& meshpath, std::shared_ptr<MeshDef>* def, std::shared_ptr<Animation>* animation);
}
