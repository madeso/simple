using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.old
{
    internal class Effect
    {
        CompositeComponent comp;

        public Effect(string file, MediaLoader ml)
        {
            XmlElement effect = Xml.Open(Xml.FromStream(ml.FS.open(file)), "effect");
            comp = new CompositeComponent(file, effect, ml);
        }

        internal void render(User usr, RenderTarget rt)
        {
            Component last = comp.getLatest();
            comp.render(new RenderArgs(usr, rt, last));
        }

        internal void update(float t)
        {
            comp.update(t);
        }
    }
}
