#pragma once

#include <memory>
#include <string>

#include "engine/fse/map.h"

namespace SimpleEngine
{
    struct MediaLoader;
}

namespace SimpleEngine::fse
{
    struct Provider;
    struct Target;
    struct Pipeline;

    struct Linker
    {
        Map<std::shared_ptr<Provider>> providers;
        Map<std::shared_ptr<Target>> targets;

        Linker();

        std::shared_ptr<Provider> getProvider(const std::string& name);
        void addProvider(std::shared_ptr<Provider> prov);

        void addTarget(std::shared_ptr<Target> targ);
        std::shared_ptr<Target> getTarget(const std::string& name);

        std::shared_ptr<Pipeline> getPipeline(std::shared_ptr<Target> target);

        void link();

        std::string read(const std::string& path, MediaLoader* ml, int width, int height);
    };
}
