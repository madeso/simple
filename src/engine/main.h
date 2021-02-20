#pragma once

#include <SDL_events.h>

#include <functional>
#include <memory>

namespace SimpleEngine
{
    struct App
    {
        virtual ~App() = default;

        virtual void OnRender(int w, int h) = 0;
        virtual void OnStep() = 0;
        virtual void OnEvent(const SDL_Event& e) = 0;
    };

    void RunMain(std::function<std::shared_ptr<App>()>&& make_app);
}
