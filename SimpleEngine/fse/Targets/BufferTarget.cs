using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Targets
{
    class BufferTarget : Target
    {
        BufferReference buffer;
        string Name
        {
            get
            {
                return Id;
            }
        }

        public override string ToString()
        {
            return base.ToString() + " targeting a buffer (" + width.ToString() + "x" + height.ToString() + ") named " + Name;
        }

        readonly int width;
        readonly int height;

        public BufferTarget(XmlElement el)
        {
            //name = Xml.GetAttributeString(el, "name");
            width = Xml.GetAttribute<int>(el, "width", int.Parse, 512);
            height = Xml.GetAttribute<int>(el, "height", int.Parse, 512);
        }

        public override void apply(Action a)
        {
            buffer.updateTexture(a);
        }

        public override int Width
        {
            get { return buffer.Width; }
        }

        public override int Height
        {
            get { return buffer.Height; }
        }

        public override void link(Linker usr)
        {
            buffer = createBuffer(Name, width, height);
        }

        
    }
}
