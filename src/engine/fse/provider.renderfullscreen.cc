#include "engine/fse/provider.renderfullscreen.h"

#include "engine/cpp.h"
#include "engine/fse/binder.h"
#include "engine/fse/target.h"
#include "engine/fullscreenquad.h"
#include "fmt/core.h"

namespace SimpleEngine::fse::Providers
{
    std::string RenderFullscreenProvider::ToString() const
    {
        const auto shader_loaded = (sh != nullptr) ? "loaded" : "";
        return fmt::format("{} renders fullscreen with {}{}", Provider::ToString(), Nullstring(shadername, "no shader"),
                           shader_loaded);
    }

    RenderFullscreenProvider::RenderFullscreenProvider(std::shared_ptr<Xml::Element> el)
        : Provider(el)
        , shadername(Xml::GetAttributeString(el, "shader"))
    {
    }

    void RenderFullscreenProvider::doProvide(RenderArgs* ra)
    {
        if (sh != nullptr)
        {
            Shader::Bind(sh);
        }
        callCommands();  // lets call the commands
        RenderFullscreenQuad(nullptr, target->Width(), target->Height());
        if (sh != nullptr)
        {
            Shader::Unbind();
        }
    }

    void RenderFullscreenProvider::doLink(Linker* user)
    {
        denyAutocallOfCommands();  // call the commands outself
    }

    void RenderFullscreenProvider::doBind(Binder* bd)
    {
        sh = bd->getShaderOrNull(shadername);
    }
}
