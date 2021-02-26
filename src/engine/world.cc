#include "engine/world.h"

#include "engine/camera.h"
#include "engine/opengl.h"
#include "engine/pushedmatrix.h"
#include "engine/renderlist.h"
#include "engine/setup.h"
#include "engine/simpleworld.h"

namespace simple
{
    std::shared_ptr<World> World::Load(MediaLoader* loader, const std::string& file)
    {
        return std::make_shared<SimpleWorld>(loader, file);
    }

    void World::Render(int width, int height, const Camera& camera)
    {
        ClearScreen();
        setup::View3d(width, height);
        {
            PushedMatrix fm;
            camera.SendRotation();
            RenderList list;
            SendCameraRenderablesToList(&list);
            list.OnRender();
        }

        glClear(GL_DEPTH_BUFFER_BIT);
        {
            PushedMatrix fm;
            camera.SendRotationAndPosition();
            RenderList list;
            SendWorldRenderablesToList(&list);
            list.OnRender();
        }
    }
}
