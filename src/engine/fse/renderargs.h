#pragma once

#include <memory>

#include "engine/camera.h"
#include "engine/world.h"

namespace simple::fse
{
    struct RenderArgs
    {
        RenderArgs(std::shared_ptr<World> ww, const Camera& c, int w, int h);

        std::shared_ptr<World> world;
        Camera camera;
        int width;
        int height;

        void Render();
    };
}
