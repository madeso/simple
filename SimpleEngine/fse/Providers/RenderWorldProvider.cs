using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Providers
{
    class RenderWorldProvider : Provider
    {
        Shader shader = null;
        readonly string shadername;

        public RenderWorldProvider(XmlElement e)
            : base(e)
        {
            shadername = Xml.GetAttributeString(e, "shader");
        }

        public override string ToString()
        {
            return base.ToString() + " renders world with " + CSharp.Nullstring(shadername,"no shader") + ((shader!=null)?"(loaded)":"");
        }

        protected override void doProvide(RenderArgs ra)
        {
            if (shader != null)
                Shader.Bind(shader);

            ra.render();

            if (shader != null)
                Shader.Unbind();
        }

        protected override void doLink(Linker user)
        {
        }

        protected override void doBind(Binder bd)
        {
            if (false == string.IsNullOrEmpty(shadername))
            {
                shader = bd.getShader(shadername);
            }
        }
    }
}
