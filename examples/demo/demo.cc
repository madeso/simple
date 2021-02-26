#include "demo/demo.h"

#include <SDL.h>

#include "engine/filesystem.h"
#include "engine/medialoader.h"
#include "engine/opengl.h"
#include "engine/quat.h"
#include "engine/rect.h"
#include "engine/setup.h"

using namespace simple;

namespace demo
{
    Demo::Demo()
        : rightleft(
              std::make_shared<PlusMinus>(std::make_shared<Hold>(ConfigurableKey(SDLK_d)), std::make_shared<Hold>(ConfigurableKey(SDLK_a))))
        , forback(
              std::make_shared<PlusMinus>(std::make_shared<Hold>(ConfigurableKey(SDLK_w)), std::make_shared<Hold>(ConfigurableKey(SDLK_s))))
        , updown(std::make_shared<PlusMinus>(std::make_shared<Hold>(ConfigurableKey(SDLK_SPACE)),
                                             std::make_shared<Hold>(ConfigurableKey(SDLK_LCTRL))))
        , sprint(std::make_shared<Hold>(ConfigurableKey(SDLK_LSHIFT)))
    {
        setup::BasicOpenGL();

        auto fs = std::make_shared<FileSystem>();
        fs->AddDefaultRoots("pretty good", "simple test");
        auto loader = MediaLoader(fs);
        // Texture sample = loader.fetch<Texture>("sample.bmp");

        world = World::Load(&loader, "level01.lvl");
        // world.add(MeshInstance(loader.fetch<Mesh>("basicroad.obj")));

        pipe = fse::Pipeline::LoadFromFile("pipeline_ssao.xml", &loader, Width, Height);
    }

    void Demo::Render()
    {
        /*fbo.updateTexture(delegate()
                {
                    world.render(Width, Height, cam);
                });*/
        auto ra = fse::RenderArgs(world, cam, Width, Height);
        pipe->Render(&ra);
        /*Shader.Bind(shader);
                FullscreenQuad.render(fbo, Width, Height);
                Shader.Unbind();*/
    }

    void Demo::OnFrame(float delta, const vec2& mouse_movement)
    {
        /*frame.begin();
                world.render(Width, Height, cam);
                frame.swap();*/
        mousesmooth = vec2::Curve(mouse_movement, mousesmooth, mousesmoothing);
        movement =
            vec3::Curve(Button::Combine(rightleft, updown, forback).GetNormalized() * (3 + sprint->Value() * 3), movement, 5);

        // math::Quaternion(math::op::vec3::yAxisPositive, -x) * math::Quaternion(mRotation.getRight(), y) *
        // mRotation;

        quat final = quat::FpsQuat(cam.rotation, mousesmooth.x * sensitivity, mousesmooth.y * sensitivity);
        cam.location = cam.location + cam.rotation.GetRUI(movement) * delta;
        cam.Rotate(final);
    }

    void Demo::OnButton(const ConfigurableKey& button, bool down)
    {
        if (button == ConfigurableKey(SDLK_ESCAPE))
            running = false;
        else
            Button::Run(button, down, {rightleft, forback, updown, sprint});
    }

    void Demo::OnBeginRender()
    {
        setup::Viewport(Rect::FromLTWH(0, 0, Width, Height));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
