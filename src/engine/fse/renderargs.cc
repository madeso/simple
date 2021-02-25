#include "engine/fse/renderargs.h"

#include <memory>

#include "engine/camera.h"
#include "engine/world.h"

namespace SimpleEngine::fse
{
    RenderArgs::RenderArgs(std::shared_ptr<SimpleEngine::World> ww, const Camera& c, int w, int h)
        : World(ww)
        , Cam(c)
        , Width(w)
        , Height(h)
    {
    }

    void RenderArgs::render()
    {
        World->Render(Width, Height, Cam);
    }
}
