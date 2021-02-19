using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Commands
{
    struct SetVec2Uniform : Command
    {
        std::string shaderName;
        Shader shader;

        std::string uniformName;
        Uniform uniform;

        vec2 vec;

        SetVec2Uniform(XmlElement el, Provider p)
            : base(el, p)
        {
            shaderName = Xml.GetAttributeString(el, "shader");
            uniformName = Xml.GetAttributeString(el, "uniform");
            float x = Xml.GetAttribute<float>(el, "x", float.Parse, 0);
            float y = Xml.GetAttribute<float>(el, "y", float.Parse, 0);

            vec = vec2(x, y);
        }

        override void apply()
        {
            uniform.bindUniform(vec);
        }

        override IEnumerable<Provider> Dependencies
        {
            get { return CSharp.Enumerate<Provider>(); }
        }

    protected
        override void doLink(Linker user)
        {
        }

    protected
        override void doBind(Binder bd)
        {
            shader = bd.getShader(shaderName);
            uniform = shader.getUniform(uniformName);
        }
    }
}
