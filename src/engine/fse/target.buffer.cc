﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine::fse.Targets
{
    struct BufferTarget : Target
    {
        BufferReference buffer;
        std::string Name
        {
            get
            {
                return Id;
            }
        }

        std::string ToString() const
        {
            return base.ToString() + " targeting a buffer (" + width.ToString() + "x" + height.ToString() + ") named " + Name;
        }

        int width;
        int height;

        BufferTarget(std::shared_ptr<Xml::Element> el)
        {
            //name = Xml::GetAttributeString(el, "name");
            width = Xml::GetAttribute<int>(el, "width", std::stoi, 512);
            height = Xml::GetAttribute<int>(el, "height", std::stoi, 512);
        }

        override void apply(Action a)
        {
            buffer.updateTexture(a);
        }

        override int Width
        {
            get { return buffer.Width; }
        }

        override int Height
        {
            get { return buffer.Height; }
        }

        override void link(Linker usr)
        {
            buffer = createBuffer(Name, width, height);
        }
    }
}
