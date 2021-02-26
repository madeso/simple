#pragma once

#include <map>
#include <memory>

#include "engine/animation.h"
#include "engine/meshdef.h"

namespace simple
{
    struct Actor
    {
        std::shared_ptr<MeshDef> mesh;
        std::map<std::string, std::shared_ptr<Animation>> animations;

        Actor(std::shared_ptr<MeshDef> m);

        std::shared_ptr<Animation> GetAnimation(const std::string& name);

        Actor& AddAnimation(const std::string& name, std::shared_ptr<Animation> an);
    };
}
