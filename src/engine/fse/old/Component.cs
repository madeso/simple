using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse.old
{
    internal abstract class Component
    {
        public abstract void update(float t);
        public abstract void render(RenderArgs ra);
        public abstract Component getLatest();
    }
}
