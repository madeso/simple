using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Providers
{
    public static class Providers
    {
        public static Provider Create(XmlElement x)
        {
            string name = x.Name;
            if (name == "fullscreen")
            {
                return new RenderFullscreenProvider(x);
            }
            else if (name == "world")
            {
                return new RenderWorldProvider(x);
            }
            else throw new Exception(name + " is not a known provider");
        }
    }
}
