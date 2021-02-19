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

        override std::string ToString()
        {
            return base.ToString() + " renders fullscreen with " + CSharp.Nullstring(shadername, "no shader") + ((sh != nullptr) ? "loaded" : "");
        }

        RenderFullscreenProvider(XmlElement el)
            : base(el)
        {
            shadername = Xml.GetAttributeString(el, "shader");
        }

    protected
        override void doProvide(RenderArgs ra)
        {
            if (sh != nullptr)
                Shader.Bind(sh);
            callCommands();  // lets call the commands
            FullscreenQuad.render(nullptr, Target.Width, Target.Height);
            if (sh != nullptr)
                Shader.Unbind();
        }

    protected
        override void doLink(Linker user)
        {
            denyAutocallOfCommands();  // call the commands outself
        }

    protected
        override void doBind(Binder bd)
        {
            sh = bd.getShaderOrNull(shadername);
        }
    }
}
