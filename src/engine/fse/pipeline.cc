using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine::fse
{
    struct Pipeline
    {
        std::vector<Provider> providers = std::vector<Provider>();

        static Pipeline Create(std::string path, MediaLoader ml, int width, int height)
        {
            Linker linker = Linker();
            std::string t = linker.read(path, ml, width, height);
            linker.link();
            Pipeline pp = linker.getPipeline(linker.getTarget(t));
            Binder bind = Binder(ml.FS);
            pp.bind(bind);
            bind.createBuffers();
            return pp;
        }

        void bind(Binder binder)
        {
            for (Provider p : providers)
            {
                p.bind(binder);
                if (p.Target != nullptr)
                {
                    p.Target.bind(binder);
                }
            }
        }

        Pipeline()
        {
        }

        void render(RenderArgs ra)
        {
            for (Provider p : providers)
            {
                p.provide(ra);
            }
        }

        void add(Provider pr)
        {
            for (Provider p : pr.Providers)
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
