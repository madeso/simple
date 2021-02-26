#include "engine/fse/pipeline.h"

#include <algorithm>

#include "engine/fse/binder.h"
#include "engine/fse/linker.h"
#include "engine/fse/target.h"
#include "engine/medialoader.h"

namespace simple::fse
{
    std::shared_ptr<Pipeline> Pipeline::LoadFromFile(const std::string& path, MediaLoader* ml, int width, int height)
    {
        Linker linker;
        std::string t = linker.ReadPipelineFromFile(path, ml, width, height);
        linker.Link();
        auto pp = linker.GetPipeline(linker.GetTarget(t));
        auto bind = Binder(ml->FS());
        pp->Bind(&bind);
        bind.CreateBuffers();
        return pp;
    }

    void Pipeline::Bind(Binder* binder)
    {
        for (auto p : providers)
        {
            p->Bind(binder);
            if (p->target != nullptr)
            {
                p->target->Bind(binder);
            }
        }
    }

    void Pipeline::Render(RenderArgs* ra)
    {
        OnSize(ra->width, ra->height);
        for (auto p : providers)
        {
            p->Provide(ra);
        }
    }

    void Pipeline::Add(std::shared_ptr<Provider> pr)
    {
        for (auto p : pr->providers)
        {
            Add(p);
        }

        if (std::find(providers.begin(), providers.end(), pr) == providers.end())
        {
            providers.emplace_back(pr);
        }
    }

    void Pipeline::OnSize(int width, int height)
    {
        for (auto p : providers)
        {
            p->OnSize(width, height);
        }
    }
}
