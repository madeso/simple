using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Providers
{
    class RenderFullscreenProvider : Provider
    {
        Shader sh;
        readonly string shadername;

        public override string ToString()
        {
            return base.ToString() + " renders fullscreen with " + CSharp.Nullstring(shadername, "no shader") + ( (sh!=null)?"loaded":"" );
        }

        public RenderFullscreenProvider(XmlElement el) : base(el)
        {
            shadername = Xml.GetAttributeString(el, "shader");
        }

        protected override void doProvide(RenderArgs ra)
        {
            if( sh != null ) Shader.Bind(sh);
            callCommands(); // lets call the commands
            FullscreenQuad.render(null, Target.Width, Target.Height);
            if (sh != null) Shader.Unbind();
        }

        protected override void doLink(Linker user)
        {
            denyAutocallOfCommands(); // call the commands outself
        }

        protected override void doBind(Binder bd)
        {
            sh = bd.getShaderOrNull(shadername);
        }
    }
}
