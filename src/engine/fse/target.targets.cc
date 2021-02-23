using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine::fse.Targets
{
    namespace Targets
    {
        static Target Create(std::shared_ptr<Xml::Element> x, int width, int height)
        {
            std::string name = x.Name;

            if (name == "buffer")
            {
                return BufferTarget(x);
            }
            else if (name == "form")
            {
                return FormTarget(width, height);
            }
            else
                throw std::runtime_error(name + " is not a known target");
        }
    }
}
