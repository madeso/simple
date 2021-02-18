using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.old
{
    internal class CompositeComponent : Component
    {
        public CompositeComponent(string path, XmlElement effect, MediaLoader ml)
        {
            foreach (XmlElement el in Xml.Elements(effect))
            {
                string name = el.Name;

                if (name == "shader")
                {
                    add(new ShaderComponent(path, el, ml));
                }
                else if (name == "composite")
                {
                    add(new CompositeComponent(path, el, ml));
                }
                else throw new Exception("Unknown fse component " + name);
            }
        }

        public void add(Component c)
        {
            components.Add(c);
        }

        List<Component> components;

        public override void update(float t)
        {
            foreach (Component c in components)
            {
                c.update(t);
            }
        }

        public override void render(RenderArgs ra)
        {
            foreach (Component c in components)
            {
                c.render(ra);
            }
        }

        public override Component getLatest()
        {
            Component t = null;
            foreach (Component p in components)
            {
                Component c = p.getLatest();
                if (c != null)
                {
                    t = c;
                }
            }
            return t;
        }
    }
}
