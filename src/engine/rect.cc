using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct Rect
    {
        static Rect FromLTWH(int l, int t, int w, int h)
        {
            Rect r = Rect();
            r.Left = l;
            r.Top = t;
            r.Width = w;
            r.Height = h;
            return r;
        }
        int Left;
        int Top;
        int Width;
        int Height;
    }
}
