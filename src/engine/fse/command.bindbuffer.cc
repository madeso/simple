using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Commands
{
    struct BindBufferCommand : Command
    {
        BufferReference buffer;
        Target targ;
        int location;
        std::string name;

        override std::string ToString()
        {
            return base.ToString() + " binds buffer " + name + " to " + location.ToString();
        }

        BindBufferCommand(XmlElement el, Provider prov)
            : base(el, prov)
        {
            name = Xml.GetAttributeString(el, "buffer");
            location = Xml.GetAttribute<int>(el, "location", int.Parse, -1);
        }

        override void apply()
        {
            buffer.bindTexture(location);
        }

        override void doLink(Linker user)
        {
            buffer = createBuffer(name);
            targ = user.getTarget(name);
        }

        override void doBind(Binder bd)
        {
            bd.reference(buffer);
        }

        override IEnumerable<Provider> Dependencies
        {
            get
            {
                yield return targ.Provider;
            }
        }
    }
}
