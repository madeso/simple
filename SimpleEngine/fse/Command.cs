using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse
{
    public abstract class Command
    {
        Provider prov;
        readonly string id;

        public Command(XmlElement el, Provider prov)
        {
            id = Xml.GetAttributeString(el, "id");
            this.prov = prov;
        }

        public override string ToString()
        {
            return id;
        }

        protected BufferReference createBuffer(string name)
        {
            return prov.createBuffer(name);
        }

        public abstract void apply();
        // should be called by our provider
        public void link(Linker linker)
        {
            doLink(linker);
        }
        public void bind(Binder b)
        {
            doBind(b);
        }

        public abstract IEnumerable<Provider> Dependencies
        {
            get;
        }

        protected abstract void doLink(Linker user);
        protected abstract void doBind(Binder bd);
    }
}
