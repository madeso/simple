using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse.Commands
{
    class BindBufferCommand : Command
    {
        BufferReference buffer;
        Target targ;
        readonly int location;
        readonly string name;

        public override string ToString()
        {
            return base.ToString() + " binds buffer " + name + " to " + location.ToString();
        }

        public BindBufferCommand(XmlElement el, Provider prov) : base(el, prov)
        {
            name = Xml.GetAttributeString(el, "buffer");
            location = Xml.GetAttribute<int>(el, "location", int.Parse, -1);
        }

        public override void apply()
        {
            buffer.bindTexture(location);
        }

        protected override void doLink(Linker user)
        {
            buffer = createBuffer(name);
            targ = user.getTarget(name);
        }

        protected override void doBind(Binder bd)
        {
            bd.reference(buffer);
        }

        public override IEnumerable<Provider> Dependencies
        {
            get
            {
                yield return targ.Provider;
            }
        }
    }
}
