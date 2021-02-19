﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    interface Renderable
    {
        void sendToRenderer(RenderList r);
        void render();
    }
}
