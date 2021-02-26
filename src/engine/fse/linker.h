#pragma once

#include <memory>
#include <string>

#include "engine/fse/map.h"

namespace simple
{
    struct MediaLoader;
}

namespace simple::fse
{
    struct Provider;
    struct Target;
    struct Pipeline;

    struct Linker
    {
        Map<std::shared_ptr<Provider>> providers;
        Map<std::shared_ptr<Target>> targets;

        Linker();

        std::shared_ptr<Provider> GetProvider(const std::string& name);
        void AddProvider(std::shared_ptr<Provider> prov);

        void AddTarget(std::shared_ptr<Target> targ);
        std::shared_ptr<Target> GetTarget(const std::string& name);

        std::shared_ptr<Pipeline> GetPipeline(std::shared_ptr<Target> target);

        void Link();

        std::string ReadPipelineFromFile(const std::string& path, MediaLoader* ml, int width, int height);
    };
}
