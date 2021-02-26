#include "engine/fse/linker.h"

#include <stdexcept>


#include "engine/filesystem.h"
#include "engine/fse/pipeline.h"
#include "engine/fse/provider.h"
#include "engine/fse/provider.providers.h"
#include "engine/fse/target.h"
#include "engine/fse/target.targets.h"
#include "engine/medialoader.h"
#include "engine/xml.h"
#include "fmt/core.h"

namespace simple::fse
{
    Linker::Linker()
        : providers([](const std::string& name) -> std::shared_ptr<Provider> {
            throw std::runtime_error(fmt::format("{} is not a defined provider", name));
        })
        , targets([](const std::string& name) -> std::shared_ptr<Target> {
            throw std::runtime_error(fmt::format("{} is not a defined target", name));
        })
    {
    }

    std::shared_ptr<Provider> Linker::GetProvider(const std::string& name)
    {
        return providers.GetOrCreate(name);
    }
    void Linker::AddProvider(std::shared_ptr<Provider> prov)
    {
        providers.Set(prov->GetId(), prov);
    }

    void Linker::AddTarget(std::shared_ptr<Target> targ)
    {
        targets.Set(targ->GetId(), targ);
    }
    std::shared_ptr<Target> Linker::GetTarget(const std::string& name)
    {
        return targets.GetOrCreate(name);
    }

    std::shared_ptr<Pipeline> Linker::GetPipeline(std::shared_ptr<Target> target)
    {
        auto pl = std::make_shared<Pipeline>();

        pl->Add(target->GetProvider());

        return pl;
    }

    void Linker::Link()
    {
        for (auto p : providers.GetCreatedValues())
        {
            Provider::Link(p, this);
        }

        for (auto t : targets.GetCreatedValues())
        {
            t->Link(this);
            if (t->GetProvider() == nullptr)
                throw std::runtime_error(fmt::format("{} is missing a provider", t->ToString()));
        }

        for (auto p : providers.GetCreatedValues())
        {
            p->OnLinkCompleted(this);
        }
    }

    std::string Linker::ReadPipelineFromFile(const std::string& path, MediaLoader* ml, int width, int height)
    {
        std::shared_ptr<xml::Element> root = xml::Open(ml->FS()->Open(path), "pipeline");
        std::string t = xml::GetAttributeString(root, "target");
        for (std::shared_ptr<xml::Element> targetElement : xml::Elements(root->GetChild("targets")))
        {
            auto target = targets::Create(targetElement, width, height);
            AddTarget(target);
        }
        for (std::shared_ptr<xml::Element> providerElement : xml::Elements(root->GetChild("providers")))
        {
            auto provider = providers::Create(providerElement);
            AddProvider(provider);
        }

        return t;
    }
}
