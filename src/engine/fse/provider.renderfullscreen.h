#pragma once

#include <memory>
#include <string>

#include "engine/fse/provider.h"
#include "engine/shader.h"
#include "engine/xml.h"

namespace simple
{
    struct ShaderProgram;
}

namespace simple::fse::providers
{
    struct RenderFullscreenProvider : Provider
    {
        std::shared_ptr<ShaderProgram> shader;
        std::string name_of_shader;

        std::string ToString() const override;

        RenderFullscreenProvider(std::shared_ptr<xml::Element> el);

        void PostProvide(RenderArgs* ra) override;

        void PostLink(Linker* user) override;

        void PreBind(Binder* bd) override;
    };
}
