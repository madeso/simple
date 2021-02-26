#pragma once

#include <string>

#include "engine/fse/provider.h"
#include "engine/xml.h"

namespace simple
{
    struct ShaderProgram;
}

namespace simple::fse::providers
{
    struct RenderWorldProvider : public Provider
    {
        std::shared_ptr<ShaderProgram> shader;
        std::string name_of_shader;

        RenderWorldProvider(std::shared_ptr<xml::Element> e);

        std::string ToString() const override;

        void PostProvide(RenderArgs* ra) override;

        void PostLink(Linker* user) override;

        void PreBind(Binder* bd) override;
    };
}
