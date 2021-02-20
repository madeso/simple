#pragma once

namespace SimpleEngine
{
    struct Rect;

    namespace Setup
    {
        void Setup();
        void BeforeRender(int Width, int Height);

        void basicOpenGL();
        void view3d(int Width, int Height);
        void Viewport(const Rect& rect);
        void view2d(int Width, int Height);
    }
}
