using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Providers
{
    struct RenderWorldProvider : Provider
    {
        Shader shader = nullptr;
        std::string shadername;

        RenderWorldProvider(XmlElement e)
            : base(e)
        {
            shadername = Xml.GetAttributeString(e, "shader");
        }

        std::string ToString() const
        {
            return base.ToString() + " renders world with " + CSharp.Nullstring(shadername, "no shader") + ((shader != nullptr) ? "(loaded)" : "");
        }

        override void doProvide(RenderArgs ra)
        {
            if (shader != nullptr)
                Shader.Bind(shader);

            ra.render();

            if (shader != nullptr)
                Shader.Unbind();
        }

        override void doLink(Linker user)
        {
        }

        override void doBind(Binder bd)
        {
            if (false == std::string.IsNullOrEmpty(shadername))
            {
                shader = bd.getShader(shadername);
            }
        }
    }
}
