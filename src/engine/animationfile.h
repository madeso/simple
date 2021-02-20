#pragma once

#include <memory>
#include <string>

#include "engine/animation.h"

namespace SimpleEngine
{
    struct FileSystem;

    namespace AnimationFile
    {
        std::shared_ptr<Animation> Load(FileSystem* fs, const std::string& path);
        void Write(std::shared_ptr<Animation> an, const std::string& s);
    }
}
