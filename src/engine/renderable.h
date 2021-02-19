#pragma once

namespace SimpleEngine
{
    struct RenderList;

    struct Renderable
    {
        virtual ~Renderable() = default;

        virtual void sendToRenderer(RenderList* r) = 0;
        virtual void render() = 0;
    };
}
