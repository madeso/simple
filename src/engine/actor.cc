#include "engine/actor.h"

namespace SimpleEngine
{
    std::shared_ptr<MeshDef> mesh;
    std::map<std::string, std::shared_ptr<Animation>> animations;

    Actor::Actor(std::shared_ptr<MeshDef> m)
        : mesh(m)
    {
    }

    std::shared_ptr<Animation> Actor::GetAnimation(const std::string& name)
    {
        return animations[name];
    }

    Actor& Actor::add(const std::string& name, std::shared_ptr<Animation> an)
    {
        animations.emplace(name, an);
        return *this;
    }
}
