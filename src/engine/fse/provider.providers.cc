#include "engine/fse/provider.providers.h"

#include <stdexcept>

#include "engine/fse/provider.renderfullscreen.h"
#include "engine/fse/provider.renderworld.h"
#include "fmt/core.h"

namespace simple::fse
{
    struct Provider;
}

namespace simple::fse::providers
{
    namespace
    {
        std::shared_ptr<Provider> PostLoad(std::shared_ptr<Provider> prov, std::shared_ptr<xml::Element> x)
        {
            Provider::PostLoad(prov, x);
            return prov;
        }
    }

    std::shared_ptr<Provider> Create(std::shared_ptr<xml::Element> x)
    {
        const auto name = x->GetName();
        if (name == "fullscreen")
        {
            return PostLoad(std::make_shared<RenderFullscreenProvider>(x), x);
        }
        else if (name == "world")
        {
            return PostLoad(std::make_shared<RenderWorldProvider>(x), x);
        }
        else
        {
            throw std::runtime_error(fmt::format("{} is not a known provider", name));
        }
    }
}
