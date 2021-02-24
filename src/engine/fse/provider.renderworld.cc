#include "engine/fse/provider.renderworld.h"

#include <string>

#include "engine/cpp.h"
#include "engine/fse/binder.h"
#include "engine/fse/provider.h"
#include "engine/fse/renderargs.h"
#include "engine/shader.h"
#include "engine/xml.h"
#include "fmt/core.h"

namespace SimpleEngine::fse::Providers
{
    RenderWorldProvider::RenderWorldProvider(std::shared_ptr<Xml::Element> e)
        : Provider(e)
    {
        shadername = Xml::GetAttributeString(e, "shader");
    }

    std::string RenderWorldProvider::ToString() const
    {
        const auto loaded_string = ((shader != nullptr) ? "(loaded)" : "");
        return fmt::format("{} renders world with {}{}", Provider::ToString(), Nullstring(shadername, "no shader"), loaded_string);
    }

    void RenderWorldProvider::doProvide(RenderArgs* ra)
    {
        if (shader != nullptr)
            Shader::Bind(shader);

        ra->render();

        if (shader != nullptr)
            Shader::Unbind();
    }

    void RenderWorldProvider::doLink(Linker* user)
    {
    }

    void RenderWorldProvider::doBind(Binder* bd)
    {
        if (shadername != "")
        {
            shader = bd->getShader(shadername);
        }
    }
}
