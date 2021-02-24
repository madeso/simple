#pragma once

#include <memory>
#include <vector>

#include "engine/fse/provider.h"
#include "engine/fse/renderargs.h"

namespace SimpleEngine
{
    struct MediaLoader;
}

namespace SimpleEngine::fse
{
    struct Pipeline
    {
        std::vector<std::shared_ptr<Provider>> providers;

        static std::shared_ptr<Pipeline> Create(const std::string& path, MediaLoader* ml, int width, int height);

        void bind(Binder* binder);

        void render(RenderArgs* ra);

        void add(std::shared_ptr<Provider> pr);
    };
}
