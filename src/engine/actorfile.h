#pragma once
#include <memory>
#include <string>

#include "engine/actor.h"

namespace simple
{
    struct FileSystem;

    namespace actor_file
    {
        std::shared_ptr<Actor> Load(FileSystem* fs, const std::string& p);
    }
}
