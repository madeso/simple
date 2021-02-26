#pragma once

#include <memory>
#include <string>

#include "engine/fse/command.h"
#include "engine/vec2.h"
#include "engine/xml.h"

namespace simple
{
    struct ShaderProgram;
    struct Uniform;
}

namespace simple::fse
{
    struct Binder;
}

namespace simple::fse::commands
{
    struct SetVec2Uniform : public Command
    {
        std::string shaderName;
        std::shared_ptr<ShaderProgram> shader;

        std::string uniformName;
        std::shared_ptr<Uniform> uniform;

        vec2 vec;

        SetVec2Uniform(std::shared_ptr<xml::Element> el, std::shared_ptr<Provider> p);

        void Apply() override;

        std::vector<std::shared_ptr<Provider>> GetDependencies() override;

        void Link(Linker* user) override;

        void Bind(Binder* bd) override;
    };
}
