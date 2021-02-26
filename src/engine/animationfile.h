#pragma once

#include <memory>
#include <string>

#include "engine/animation.h"

namespace simple
{
    struct FileSystem;

    namespace animation_file
    {
        std::shared_ptr<Animation> Load(FileSystem* fs, const std::string& path);
        void Write(std::shared_ptr<Animation> an, const std::string& s);
    }
}
