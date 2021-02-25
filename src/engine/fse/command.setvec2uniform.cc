#include "engine/fse/command.setvec2uniform.h"

#include "engine/fse/binder.h"
#include "engine/math1.h"
#include "engine/shader.h"

namespace SimpleEngine::fse::Commands
{
    SetVec2Uniform::SetVec2Uniform(std::shared_ptr<Xml::Element> el, std::shared_ptr<Provider> p)
        : Command(el, p)
    {
        shaderName = Xml::GetAttributeString(el, "shader");
        uniformName = Xml::GetAttributeString(el, "uniform");
        float x = Xml::GetAttribute<float>(el, "x", math1::ParseFloat, 0);
        float y = Xml::GetAttribute<float>(el, "y", math1::ParseFloat, 0);

        vec = vec2(x, y);
    }

    void SetVec2Uniform::apply()
    {
        uniform->BindUniform(vec);
    }

    std::vector<std::shared_ptr<Provider>> SetVec2Uniform::Dependencies()
    {
        return {};
    }

    void SetVec2Uniform::doLink(Linker* user)
    {
    }

    void SetVec2Uniform::doBind(Binder* bd)
    {
        shader = bd->getShader(shaderName);
        uniform = shader->GetUniformFromName(uniformName);
    }
}
