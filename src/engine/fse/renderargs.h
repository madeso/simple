#pragma once

#include <memory>

#include "engine/camera.h"
#include "engine/world.h"

namespace SimpleEngine::fse
{
    struct RenderArgs
    {
        RenderArgs(std::shared_ptr<World> ww, const Camera& c, int w, int h);

        std::shared_ptr<World> World;
        Camera Cam;
        int Width;
        int Height;

        void render();
    };
}
