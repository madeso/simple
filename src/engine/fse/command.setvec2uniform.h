#pragma once

#include <memory>
#include <string>

#include "engine/fse/command.h"
#include "engine/vec2.h"
#include "engine/xml.h"

namespace SimpleEngine
{
    struct Shader;
    struct Uniform;
}

namespace SimpleEngine::fse
{
    struct Binder;
}

namespace SimpleEngine::fse::Commands
{
    struct SetVec2Uniform : public Command
    {
        std::string shaderName;
        std::shared_ptr<Shader> shader;

        std::string uniformName;
        std::shared_ptr<Uniform> uniform;

        vec2 vec;

        SetVec2Uniform(std::shared_ptr<Xml::Element> el, std::shared_ptr<Provider> p);

        void apply() override;

        std::vector<std::shared_ptr<Provider>> Dependencies() override;

        void doLink(Linker* user) override;

        void doBind(Binder* bd) override;
    };
}
