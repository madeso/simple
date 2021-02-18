using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse.old
{
    public class User
    {
        public User()
        {
        }

        public void add(Effect e)
        {
            if (effects.Contains(e) == true)
            {
                throw new Exception("Effect already added");
            }
            effects.Add(e);
        }

        public void remove(Effect e)
        {
            if (effects.Contains(e) == false)
            {
                throw new Exception("Effect not added");
            }
            effects.Remove(e);
            active.Remove(e);
        }

        public void disable(Effect e)
        {
            if (effects.Contains(e) == false)
            {
                throw new Exception("Effect not added");
            }

            active.Remove(e);
        }

        public void Enable(Effect e)
        {
            if (effects.Contains(e) == false)
            {
                throw new Exception("Effect not added");
            }
            active.Add(e);
        }

        public void update(float t)
        {
            foreach (Effect e in active)
            {
                e.update(t);
            }
        }

        public void render(RenderTarget rt)
        {
            foreach (Effect e in active)
            {
                e.render(this, rt);
            }
        }

        List<Effect> effects;
        List<Effect> active;

        Dictionary<string, Fbo> fbos;

        internal Fbo getBuffer(string buffer)
        {
            if (fbos.ContainsKey(buffer))
            {
                return fbos[buffer];
            }

            Fbo fbo = new Fbo(512, 512, false);
            fbos.Add(buffer, fbo);
            return fbo;
        }

        internal void apply(Shader shader, string targetName)
        {
            Fbo target = getBuffer(targetName);

            target.updateTexture(delegate()
            {
                Shader.Bind(shader);
                FullscreenQuad.render(null, target.Width, target.Height);
                Shader.Unbind();
            });
        }
    }
}
