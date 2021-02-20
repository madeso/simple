#include "engine/rect.h"

namespace SimpleEngine
{
    Rect Rect::FromLTWH(int l, int t, int w, int h)
    {
        Rect r;
        r.Left = l;
        r.Top = t;
        r.Width = w;
        r.Height = h;
        return r;
    }
}
