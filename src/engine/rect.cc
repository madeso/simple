#include "engine/rect.h"

namespace simple
{
    Rect Rect::FromLTWH(int l, int t, int w, int h)
    {
        Rect r;
        r.left = l;
        r.top = t;
        r.width = w;
        r.height = h;
        return r;
    }
}
