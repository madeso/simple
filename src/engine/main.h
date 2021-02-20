#pragma once

#include <functional>
#include <memory>

namespace SimpleEngine
{
    struct App
    {
        virtual ~App() = default;

        virtual void OnRender() = 0;
        virtual void OnStep() = 0;
    };

    void RunMain(std::function<std::shared_ptr<App>()>&& make_app);
}
