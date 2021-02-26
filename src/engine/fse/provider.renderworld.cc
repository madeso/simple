#include "engine/fse/provider.renderworld.h"

#include <string>

#include "engine/strings.h"
#include "engine/fse/binder.h"
#include "engine/fse/provider.h"
#include "engine/fse/renderargs.h"
#include "engine/shader.h"
#include "engine/xml.h"
#include "fmt/core.h"

namespace simple::fse::providers
{
    RenderWorldProvider::RenderWorldProvider(std::shared_ptr<xml::Element> e)
        : Provider(e)
        , name_of_shader(xml::GetAttributeString(e, "shader"))
    {
    }

    std::string RenderWorldProvider::ToString() const
    {
        const auto loaded_string = ((shader != nullptr) ? "(loaded)" : "");
        return fmt::format("{} renders world with {}{}", Provider::ToString(), Nullstring(name_of_shader, "no shader"),
                           loaded_string);
    }

    void RenderWorldProvider::PostProvide(RenderArgs* ra)
    {
        if (shader != nullptr)
            ShaderProgram::Bind(shader);

        ra->Render();

        if (shader != nullptr)
            ShaderProgram::Unbind();
    }

    void RenderWorldProvider::PostLink(Linker* user)
    {
    }

    void RenderWorldProvider::PreBind(Binder* bd)
    {
        if (name_of_shader != "")
        {
            shader = bd->GetShader(name_of_shader);
        }
    }
}
