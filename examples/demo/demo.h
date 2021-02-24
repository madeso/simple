#pragma once

#include <memory>

#include "engine/camera.h"
#include "engine/ckey.h"
#include "engine/fse/pipeline.h"
#include "engine/key.h"
#include "engine/vec2.h"
#include "engine/vec3.h"
#include "engine/window.h"
#include "engine/world.h"

using namespace SimpleEngine;

namespace SimpleTest
{
    struct Demo : public Window
    {
        Camera cam;

        bool running = true;

        vec2 mousesmooth = vec2(0, 0);
        vec3 movement = vec3(0, 0, 0);

        float sensitivity = 0.1f;
        float mousesmoothing = 6;

        std::shared_ptr<Key> rightleft;
        std::shared_ptr<Key> forback;
        std::shared_ptr<Key> updown;
        std::shared_ptr<Hold> sprint;

        std::shared_ptr<World> world;
        std::shared_ptr<fse::Pipeline> pipe;

        Demo();

        void Render();

        void OnFrame(float delta, const vec2& mouse_movement);

        void OnButton(const Ckey& button, bool down);

        void OnBeginRender();
    };
}
