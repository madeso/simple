#include "engine/actor.h"

namespace simple
{
    Actor::Actor(std::shared_ptr<MeshDef> m)
        : mesh(m)
    {
    }

    std::shared_ptr<Animation> Actor::GetAnimation(const std::string& name)
    {
        return animations[name];
    }

    Actor& Actor::AddAnimation(const std::string& name, std::shared_ptr<Animation> an)
    {
        animations.emplace(name, an);
        return *this;
    }
}
