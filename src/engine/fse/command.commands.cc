using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Commands
{
    namespace Commands
    {
        static Command Create(XmlElement el, Provider pro)
        {
            std::string name = el.Name;
            if (name == "bindbuffer")
                return BindBufferCommand(el, pro);
            else if (name == "setu2")
                return SetVec2Uniform(el, pro);
            else
                throw Exception(name + " is not a valid command");
        }
    }
}
