using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.old
{
    abstract class Updator
    {
        public abstract void apply(Shader sh, RenderArgs ra);
        public abstract void udate(float t);
    }
    class ApplyTexture : Updator
    {
        Texture text;
        readonly int location;

        public ApplyTexture(XmlElement el, MediaLoader ml)
        {
            string file = Xml.GetAttributeString(el, "file");
            location = Xml.GetAttribute<int>(el, "location", int.Parse, -1);

            text = ml.fetch<Texture>(file);
        }

        public override void apply(Shader sh, RenderArgs ra)
        {
            text.bind(location);
        }

        public override void udate(float t)
        {
        }
    }

    class ApplyBuffer : Updator
    {
        readonly string buffer;
        readonly int location;

        public ApplyBuffer(XmlElement el, MediaLoader ml)
        {
            buffer = Xml.GetAttributeString(el, "buffer");
            location = Xml.GetAttribute<int>(el, "location", int.Parse, -1);
        }

        public override void apply(Shader sh, RenderArgs ra)
        {
            Fbo buff = ra.getBuffer(buffer);
            buff.bindTexture(location);
        }

        public override void udate(float t)
        {
        }
    }
    class UpdateFloat : Updator
    {
        readonly string name;
        readonly float times;
        readonly Uniform uniform;
        float acc;

        public UpdateFloat(XmlElement el, Shader sh)
        {
            name = Xml.GetAttributeString(el, "name");
            times = Xml.GetAttribute<float>(el, "speed", float.Parse, 1.0f);
            uniform = sh.getUniform(name);

            acc = 0;
        }

        public override void apply(Shader sh, RenderArgs ra)
        {
            uniform.bindUniform(acc);
        }

        public override void udate(float t)
        {
            acc = math1.Wrap(0, acc + t * times, 1);
        }
    }

    internal class ShaderComponent : Component
    {
        Shader shader;

        readonly string name;
        readonly string target;

        public ShaderComponent(string path, XmlElement el, MediaLoader ml)
        {
            name = Xml.GetAttributeString(el, "id");
            target = Xml.GetAttributeString(el, "target");

            XmlElement sh = el["shader"];
            shader = new Shader(path + ": " + name, sh);

            foreach (XmlElement bind in el["binds"])
            {
                updates.Add(new UpdateFloat(bind, shader));
            }

            foreach (XmlElement upd in el["update"])
            {
                string updname = upd.Name;
                if (updname == "texture")
                {
                    updates.Add(new ApplyTexture(upd, ml));
                }
                else if (updname == "buffer")
                {
                    updates.Add(new ApplyBuffer(upd, ml));
                }
            }
        }

        List<Updator> updates = new List<Updator>();

        public override void update(float t)
        {
            foreach (Updator u in updates)
            {
                u.udate(t);
            }
        }

        public override void render(RenderArgs ra)
        {
            foreach (Updator u in updates)
            {
                u.apply(shader, ra);
            }

            ra.apply(shader, target, this);
        }

        public override Component getLatest()
        {
            return this;
        }
    }
}
