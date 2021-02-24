#include "engine/fse/pipeline.h"

#include <algorithm>

#include "engine/fse/binder.h"
#include "engine/fse/linker.h"
#include "engine/fse/target.h"
#include "engine/medialoader.h"

namespace SimpleEngine::fse
{
    std::shared_ptr<Pipeline> Pipeline::Create(const std::string& path, MediaLoader* ml, int width, int height)
    {
        Linker linker;
        std::string t = linker.read(path, ml, width, height);
        linker.link();
        auto pp = linker.getPipeline(linker.getTarget(t));
        auto bind = Binder(ml->FS());
        pp->bind(&bind);
        bind.createBuffers();
        return pp;
    }

    void Pipeline::bind(Binder* binder)
    {
        for (auto p : providers)
        {
            p->bind(binder);
            if (p->target != nullptr)
            {
                p->target->bind(binder);
            }
        }
    }

    void Pipeline::render(RenderArgs* ra)
    {
        for (auto p : providers)
        {
            p->provide(ra);
        }
    }

    void Pipeline::add(std::shared_ptr<Provider> pr)
    {
        for (auto p : pr->providers)
        {
            add(p);
        }

        if (std::find(providers.begin(), providers.end(), pr) == providers.end())
        {
            providers.emplace_back(pr);
        }
    }
}
