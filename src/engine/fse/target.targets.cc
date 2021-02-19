﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Targets
{
    namespace Targets
    {
        static Target Create(XmlElement x, int width, int height)
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
                throw Exception(name + " is not a known target");
        }
    }
}
