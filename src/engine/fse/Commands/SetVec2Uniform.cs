using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Commands
{
    class SetVec2Uniform : Command
    {
        readonly string shaderName;
        Shader shader;
        
        readonly string uniformName;
        Uniform uniform;

        readonly vec2 vec;

        public SetVec2Uniform(XmlElement el, Provider p)
            : base(el, p)
        {
            shaderName = Xml.GetAttributeString(el, "shader");
            uniformName = Xml.GetAttributeString(el, "uniform");
            float x = Xml.GetAttribute<float>(el, "x", float.Parse, 0);
            float y = Xml.GetAttribute<float>(el, "y", float.Parse, 0);

            vec = new vec2(x, y);
        }


        public override void apply()
        {
            uniform.bindUniform(vec);
        }

        public override IEnumerable<Provider> Dependencies
        {
            get { return CSharp.Enumerate<Provider>(); }
        }

        protected override void doLink(Linker user)
        {
        }

        protected override void doBind(Binder bd)
        {
            shader = bd.getShader(shaderName);
            uniform = shader.getUniform(uniformName);
        }
    }
}
