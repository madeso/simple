#pragma once

namespace simple
{
    struct Rect;

    namespace setup
    {
        void Setup();
        void BeforeRender(int Width, int Height);

        void BasicOpenGL();
        void View3d(int Width, int Height);
        void Viewport(const Rect& rect);
        void View2d(int Width, int Height);
    }
}
