#pragma once

namespace simple
{
    struct Rect
    {
        static Rect FromLTWH(int l, int t, int w, int h);

        int left;
        int top;
        int width;
        int height;
    };
}
