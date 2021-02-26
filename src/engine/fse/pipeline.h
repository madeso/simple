#pragma once

#include <memory>
#include <vector>

#include "engine/fse/provider.h"
#include "engine/fse/renderargs.h"

namespace simple
{
    struct MediaLoader;
}

namespace simple::fse
{
    struct Pipeline
    {
        std::vector<std::shared_ptr<Provider>> providers;

        static std::shared_ptr<Pipeline> LoadFromFile(const std::string& path, MediaLoader* ml, int width, int height);

        void Bind(Binder* binder);

        void Render(RenderArgs* ra);

        void Add(std::shared_ptr<Provider> pr);

        void OnSize(int width, int height);
    };
}
