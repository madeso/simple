using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Providers
{
    namespace Providers
    {
        static Provider Create(XmlElement x)
        {
            std::string name = x.Name;
            if (name == "fullscreen")
            {
                return RenderFullscreenProvider(x);
            }
            else if (name == "world")
            {
                return RenderWorldProvider(x);
            }
            else
                throw Exception(name + " is not a known provider");
        }
    }
}
