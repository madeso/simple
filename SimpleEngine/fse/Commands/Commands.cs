using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Commands
{
    public static class Commands
    {
        public static Command Create(XmlElement el, Provider pro)
        {
            string name = el.Name;
            if (name == "bindbuffer") return new BindBufferCommand(el, pro);
            else if (name == "setu2") return new SetVec2Uniform(el, pro);
            else throw new Exception(name + " is not a valid command");
        }
    }
}
