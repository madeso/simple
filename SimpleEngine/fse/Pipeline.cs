using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    public class Pipeline
    {
        private List<Provider> providers = new List<Provider>();

        public static Pipeline Create(string path, MediaLoader ml, int width, int height)
        {
            Linker linker = new Linker();
            string t = linker.read(path, ml, width, height);
            linker.link();
            Pipeline pp = linker.getPipeline( linker.getTarget(t) );
            Binder bind = new Binder(ml.FS);
            pp.bind(bind);
            bind.createBuffers();
            return pp;
        }

        private void bind(Binder binder)
        {
            foreach (Provider p in providers)
            {
                p.bind(binder);
                if (p.Target != null)
                {
                    p.Target.bind(binder);
                }
            }
        }

        internal Pipeline()
        {
        }

        public void render(RenderArgs ra)
        {
            foreach (Provider p in providers)
            {
                p.provide(ra);
            }
        }

        internal void add(Provider pr)
        {
            foreach (Provider p in pr.Providers)
            {
                add(p);
            }
            
            if (false == providers.Contains(pr))
            {
                providers.Add(pr);
            }
        }
    }
}
