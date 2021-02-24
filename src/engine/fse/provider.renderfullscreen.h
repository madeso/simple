#pragma once

#include <memory>
#include <string>

#include "engine/fse/provider.h"
#include "engine/shader.h"
#include "engine/xml.h"

namespace SimpleEngine
{
    struct Shader;
}

namespace SimpleEngine::fse::Providers
{
    struct RenderFullscreenProvider : Provider
    {
        std::shared_ptr<Shader> sh;
        std::string shadername;

        std::string ToString() const override;

        RenderFullscreenProvider(std::shared_ptr<Xml::Element> el);

        void doProvide(RenderArgs* ra) override;

        void doLink(Linker* user) override;

        void doBind(Binder* bd) override;
    };
}
