#include "engine/fse/renderargs.h"

#include <memory>

#include "engine/camera.h"
#include "engine/world.h"

namespace simple::fse
{
    RenderArgs::RenderArgs(std::shared_ptr<simple::World> ww, const Camera& c, int w, int h)
        : world(ww)
        , camera(c)
        , width(w)
        , height(h)
    {
    }

    void RenderArgs::Render()
    {
        world->Render(width, height, camera);
    }
}
