#pragma once

namespace simple
{
    struct RenderList;

    struct Renderable
    {
        virtual ~Renderable() = default;

        virtual void SendToRenderer(RenderList* r) = 0;
        virtual void OnRender() = 0;
    };
}
