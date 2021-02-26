#include "engine/fse/provider.renderfullscreen.h"

#include "engine/strings.h"
#include "engine/fse/binder.h"
#include "engine/fse/target.h"
#include "engine/fullscreenquad.h"
#include "fmt/core.h"

namespace simple::fse::providers
{
    std::string RenderFullscreenProvider::ToString() const
    {
        const auto shader_loaded = (shader != nullptr) ? "loaded" : "";
        return fmt::format("{} renders fullscreen with {}{}", Provider::ToString(), Nullstring(name_of_shader, "no shader"),
                           shader_loaded);
    }

    RenderFullscreenProvider::RenderFullscreenProvider(std::shared_ptr<xml::Element> el)
        : Provider(el)
        , name_of_shader(xml::GetAttributeString(el, "shader"))
    {
    }

    void RenderFullscreenProvider::PostProvide(RenderArgs* ra)
    {
        if (shader != nullptr)
        {
            ShaderProgram::Bind(shader);
        }
        CallCommands();  // lets call the commands
        RenderFullscreenQuad(nullptr, target->GetWidth(), target->GetHeight());
        if (shader != nullptr)
        {
            ShaderProgram::Unbind();
        }
    }

    void RenderFullscreenProvider::PostLink(Linker* user)
    {
        DenyAutocallOfCommands();  // call the commands outself
    }

    void RenderFullscreenProvider::PreBind(Binder* bd)
    {
        shader = bd->GetShaderOrNull(name_of_shader);
    }
}
