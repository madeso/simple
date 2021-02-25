#include "engine/world.h"

#include "engine/camera.h"
#include "engine/opengl.h"
#include "engine/pushedmatrix.h"
#include "engine/renderlist.h"
#include "engine/setup.h"
#include "engine/simpleworld.h"

namespace SimpleEngine
{
    std::shared_ptr<World> World::Load(MediaLoader* loader, const std::string& file)
    {
        return std::make_shared<SimpleWorld>(loader, file);
    }

    void World::render(int width, int height, const Camera& c)
    {
        clearScreen();
        Setup::view3d(width, height);
        {
            PushedMatrix fm;
            c.sendRotation();
            RenderList list;
            cameraSendTo(&list);
            list.render();
        }

        glClear(GL_DEPTH_BUFFER_BIT);
        {
            PushedMatrix fm;
            c.sendRotationAndPosition();
            RenderList list;
            worldSendTo(&list);
            list.render();
        }
    }
}
