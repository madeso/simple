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

namespace SimpleTest
{
    struct Demo : public SimpleEngine::Window
    {
        SimpleEngine::Camera cam;

        bool running = true;

        SimpleEngine::vec2 mousesmooth = SimpleEngine::vec2(0, 0);
        SimpleEngine::vec3 movement = SimpleEngine::vec3(0, 0, 0);

        float sensitivity = 0.1f;
        float mousesmoothing = 6;

        std::shared_ptr<SimpleEngine::Key> rightleft;
        std::shared_ptr<SimpleEngine::Key> forback;
        std::shared_ptr<SimpleEngine::Key> updown;
        std::shared_ptr<SimpleEngine::Hold> sprint;

        std::shared_ptr<SimpleEngine::World> world;
        std::shared_ptr<SimpleEngine::fse::Pipeline> pipe;

        Demo();

        void Render();

        void OnFrame(float delta, const SimpleEngine::vec2& mouse_movement);

        void OnButton(const SimpleEngine::Ckey& button, bool down);

        void OnBeginRender();
    };
}
