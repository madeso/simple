#pragma once

#include <SDL.h>

#include <functional>
#include <memory>

namespace simple
{
    struct App
    {
        bool run = true;
        bool use_mouse = true;
        bool use_keyboard = true;
        bool use_imgui = true;

        virtual ~App() = default;

        virtual void OnRender(int w, int h) = 0;
        virtual void OnStep(float dt) = 0;
        virtual void OnEvent(const SDL_Event& e) = 0;
    };

    void RunMain(std::function<std::shared_ptr<App>()>&& make_app);
}
