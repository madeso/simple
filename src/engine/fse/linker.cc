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

namespace SimpleEngine::fse
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

    std::shared_ptr<Provider> Linker::getProvider(const std::string& name)
    {
        return providers.get(name);
    }
    void Linker::addProvider(std::shared_ptr<Provider> prov)
    {
        providers.add(prov->Id(), prov);
    }

    void Linker::addTarget(std::shared_ptr<Target> targ)
    {
        targets.add(targ->Id(), targ);
    }
    std::shared_ptr<Target> Linker::getTarget(const std::string& name)
    {
        return targets.get(name);
    }

    std::shared_ptr<Pipeline> Linker::getPipeline(std::shared_ptr<Target> target)
    {
        auto pl = std::make_shared<Pipeline>();

        pl->add(target->Provider());

        return pl;
    }

    void Linker::link()
    {
        for (auto p : providers.Data())
        {
            Provider::link(p, this);
        }

        for (auto t : targets.Data())
        {
            t->link(this);
            if (t->Provider() == nullptr)
                throw std::runtime_error(fmt::format("{} is missing a provider", t->ToString()));
        }

        for (auto p : providers.Data())
        {
            p->postlink(this);
        }
    }

    std::string Linker::read(const std::string& path, MediaLoader* ml, int width, int height)
    {
        std::shared_ptr<Xml::Element> root = Xml::Open(ml->FS()->open(path), "pipeline");
        std::string t = Xml::GetAttributeString(root, "target");
        for (std::shared_ptr<Xml::Element> targetElement : Xml::Elements(root->GetChild("targets")))
        {
            auto target = Targets::Create(targetElement, width, height);
            addTarget(target);
        }
        for (std::shared_ptr<Xml::Element> providerElement : Xml::Elements(root->GetChild("providers")))
        {
            auto provider = Providers::Create(providerElement);
            addProvider(provider);
        }

        return t;
    }
}
