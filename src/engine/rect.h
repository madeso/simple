#pragma once

namespace SimpleEngine
{
    struct Rect
    {
        static Rect FromLTWH(int l, int t, int w, int h);

        int Left;
        int Top;
        int Width;
        int Height;
    };
}
