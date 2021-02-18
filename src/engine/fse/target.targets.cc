using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Targets
{
    public static class Targets
    {
        public static Target Create(XmlElement x, int width, int height)
        {
            string name = x.Name;

            if (name == "buffer")
            {
                return new BufferTarget(x);
            }
            else if (name == "form")
            {
                return new FormTarget(width, height);
            }
            else throw new Exception(name + " is not a known target");
        }
    }
}
