#pragma once

#include <memory>
#include <string>

namespace simple
{
    struct FileSystem;
    struct MeshDef;
    struct Animation;
}

namespace simple::load::milkshape_binary
{
    void Load(FileSystem* fs, const std::string& meshpath, std::shared_ptr<MeshDef>* def, std::shared_ptr<Animation>* animation);
}
