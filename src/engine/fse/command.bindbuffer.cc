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

        std::string ToString() const
        {
            return base.ToString() + " binds buffer " + name + " to " + location.ToString();
        }

        BindBufferCommand(std::shared_ptr<Xml::Element> el, Provider prov)
            : base(el, prov)
        {
            name = Xml::GetAttributeString(el, "buffer");
            location = Xml::GetAttribute<int>(el, "location", std::stoi, -1);
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
