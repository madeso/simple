#include "engine/fse/provider.providers.h"

#include <stdexcept>

#include "engine/fse/provider.renderfullscreen.h"
#include "engine/fse/provider.renderworld.h"
#include "fmt/core.h"

namespace SimpleEngine::fse
{
    struct Provider;
}

namespace SimpleEngine::fse::Providers
{
    std::shared_ptr<Provider> Create(std::shared_ptr<Xml::Element> x)
    {
        const auto name = x->GetName();
        if (name == "fullscreen")
        {
            return std::make_shared<RenderFullscreenProvider>(x);
        }
        else if (name == "world")
        {
            return std::make_shared<RenderWorldProvider>(x);
        }
        else
        {
            throw std::runtime_error(fmt::format("{} is not a known provider", name));
        }
    }
}
