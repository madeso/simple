using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine::fse.Providers
{
    namespace Providers
    {
        static Provider Create(std::shared_ptr<Xml::Element> x)
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
                throw std::runtime_error(name + " is not a known provider");
        }
    }
}
