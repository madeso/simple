#pragma once

#include <string>

#include "engine/fse/provider.h"
#include "engine/xml.h"

namespace SimpleEngine
{
    struct Shader;
}

namespace SimpleEngine::fse::Providers
{
    struct RenderWorldProvider : public Provider
    {
        std::shared_ptr<Shader> shader;
        std::string shadername;

        RenderWorldProvider(std::shared_ptr<Xml::Element> e);

        std::string ToString() const override;

        void doProvide(RenderArgs* ra) override;

        void doLink(Linker* user) override;

        void doBind(Binder* bd) override;
    };
}
