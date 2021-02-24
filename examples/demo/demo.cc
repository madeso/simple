#include <SDL2/SDL.h>

#include <memory>
#include <vector>

#include "engine/camera.h"
#include "engine/ckey.h"
#include "engine/filesystem.h"
#include "engine/fse/pipeline.h"
#include "engine/key.h"
#include "engine/medialoader.h"
#include "engine/opengl.h"
#include "engine/quat.h"
#include "engine/rect.h"
#include "engine/setup.h"
#include "engine/vec2.h"
#include "engine/vec3.h"
#include "engine/window.h"
#include "engine/world.h"

using namespace SimpleEngine;

namespace SimpleTest
{
    struct Demo : public Window
    {
        Camera cam = Camera();

        bool running = true;

        vec2 mousesmooth = vec2(0, 0);
        vec3 movement = vec3(0, 0, 0);

        float sensitivity = 0.1f;
        float mousesmoothing = 6;

        std::shared_ptr<Key> rightleft =
            std::make_shared<PlusMinus>(std::make_shared<Hold>(Ckey(SDLK_d)), std::make_shared<Hold>(Ckey(SDLK_a)));
        std::shared_ptr<Key> forback =
            std::make_shared<PlusMinus>(std::make_shared<Hold>(Ckey(SDLK_w)), std::make_shared<Hold>(Ckey(SDLK_s)));
        std::shared_ptr<Key> updown = std::make_shared<PlusMinus>(std::make_shared<Hold>(Ckey(SDLK_SPACE)),
                                                                  std::make_shared<Hold>(Ckey(SDLK_LCTRL)));
        std::shared_ptr<Hold> sprint = std::make_shared<Hold>(Ckey(SDLK_LSHIFT));

        std::shared_ptr<World> world;
        std::shared_ptr<fse::Pipeline> pipe;

        Demo()
        {
            Setup::basicOpenGL();

            auto fs = std::make_shared<FileSystem>();
            fs->addDefaultRoots("pretty good", "simple test");
            auto loader = MediaLoader(fs);
            // Texture sample = loader.fetch<Texture>("sample.bmp");

            world = World::Load(&loader, "level01.lvl");
            // world.add(MeshInstance(loader.fetch<Mesh>("basicroad.obj")));

            pipe = fse::Pipeline::Create("pipeline.xml", &loader, Width, Height);
        }

        void Render()
        {
            /*fbo.updateTexture(delegate()
                    {
                        world.render(Width, Height, cam);
                    });*/
            auto ra = fse::RenderArgs(world, cam, Width, Height);
            pipe->render(&ra);
            /*Shader.Bind(shader);
                    FullscreenQuad.render(fbo, Width, Height);
                    Shader.Unbind();*/
        }

        void OnFrame(float delta, const vec2& mouse_movement)
        {
            /*frame.begin();
                    world.render(Width, Height, cam);
                    frame.swap();*/
            mousesmooth = vec2::Curve(mouse_movement, mousesmooth, mousesmoothing);
            movement = vec3::Curve(Key::Combine(rightleft, updown, forback).Normalized() * (3 + sprint->Value() * 3),
                                   movement, 5);

            // math::Quaternion(math::op::vec3::yAxisPositive, -x) * math::Quaternion(mRotation.getRight(), y) *
            // mRotation;

            quat final = quat::FpsQuat(cam.rotation, mousesmooth.x * sensitivity, mousesmooth.y * sensitivity);
            cam.location = cam.location + cam.rotation.getRUI(movement) * delta;
            cam.rotate(final);
        }

        void OnButton(const Ckey& button, bool down)
        {
            if (button == Ckey(SDLK_ESCAPE))
                running = false;
            else
                Key::Run(button, down, {rightleft, forback, updown, sprint});
        }

        void OnBeginRender()
        {
            Setup::Viewport(Rect::FromLTWH(0, 0, Width, Height));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    };
}
