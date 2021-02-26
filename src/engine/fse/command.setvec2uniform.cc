#include "engine/fse/command.setvec2uniform.h"

#include "engine/fse/binder.h"
#include "engine/math.h"
#include "engine/shader.h"

namespace simple::fse::commands
{
    SetVec2Uniform::SetVec2Uniform(std::shared_ptr<xml::Element> el, std::shared_ptr<Provider> p)
        : Command(el, p)
    {
        shaderName = xml::GetAttributeString(el, "shader");
        uniformName = xml::GetAttributeString(el, "uniform");
        float x = xml::GetAttribute<float>(el, "x", math::ParseFloat, 0);
        float y = xml::GetAttribute<float>(el, "y", math::ParseFloat, 0);

        vec = vec2(x, y);
    }

    void SetVec2Uniform::Apply()
    {
        uniform->Set(vec);
    }

    std::vector<std::shared_ptr<Provider>> SetVec2Uniform::GetDependencies()
    {
        return {};
    }

    void SetVec2Uniform::Link(Linker* user)
    {
    }

    void SetVec2Uniform::Bind(Binder* bd)
    {
        shader = bd->GetShader(shaderName);
        uniform = shader->GetUniformFromName(uniformName);
    }
}
