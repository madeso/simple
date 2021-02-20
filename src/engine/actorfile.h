#pragma once
#include <memory>
#include <string>

#include "engine/actor.h"

namespace SimpleEngine
{
    struct FileSystem;

    namespace ActorFile
    {
        std::shared_ptr<Actor> Load(FileSystem* fs, const std::string& p);
    }
}
