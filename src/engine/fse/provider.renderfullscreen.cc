using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Providers
{
    struct RenderFullscreenProvider : Provider
    {
        Shader sh;
        std::string shadername;

        std::string ToString() const
        {
            return base.ToString() + " renders fullscreen with " + CSharp.Nullstring(shadername, "no shader") + ((sh != nullptr) ? "loaded" : "");
        }

        RenderFullscreenProvider(std::shared_ptr<Xml::Element> el)
            : base(el)
        {
            shadername = Xml::GetAttributeString(el, "shader");
        }

        override void doProvide(RenderArgs ra)
        {
            if (sh != nullptr)
                Shader.Bind(sh);
            callCommands();  // lets call the commands
            FullscreenQuad.render(nullptr, Target.Width, Target.Height);
            if (sh != nullptr)
                Shader.Unbind();
        }

        override void doLink(Linker user)
        {
            denyAutocallOfCommands();  // call the commands outself
        }

        override void doBind(Binder bd)
        {
            sh = bd.getShaderOrNull(shadername);
        }
    }
}
