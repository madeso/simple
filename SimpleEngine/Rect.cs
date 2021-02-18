using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public struct Rect
    {
        public static Rect FromLTWH(int l, int t, int w, int h)
        {
            Rect r = new Rect();
            r.Left = l;
            r.Top = t;
            r.Width = w;
            r.Height = h;
            return r;
        }
        public int Left;
        public int Top;
        public int Width;
        public int Height;
    }
}
