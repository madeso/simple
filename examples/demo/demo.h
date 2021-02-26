#pragma once

#include <memory>

#include "engine/camera.h"
#include "engine/configurablekey.h"
#include "engine/fse/pipeline.h"
#include "engine/key.h"
#include "engine/vec2.h"
#include "engine/vec3.h"
#include "engine/window.h"
#include "engine/world.h"

namespace demo
{
    struct Demo : public simple::Window
    {
        simple::Camera cam;

        bool running = true;

        simple::vec2 mousesmooth = simple::vec2(0, 0);
        simple::vec3 movement = simple::vec3(0, 0, 0);

        float sensitivity = 0.1f;
        float mousesmoothing = 6;

        std::shared_ptr<simple::Button> rightleft;
        std::shared_ptr<simple::Button> forback;
        std::shared_ptr<simple::Button> updown;
        std::shared_ptr<simple::Hold> sprint;

        std::shared_ptr<simple::World> world;
        std::shared_ptr<simple::fse::Pipeline> pipe;

        Demo();

        void Render();

        void OnFrame(float delta, const simple::vec2& mouse_movement);

        void OnButton(const simple::ConfigurableKey& button, bool down);

        void OnBeginRender();
    };
}
