using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse
{
    struct Command
    {
        Provider prov;
        std::string id;

        Command(XmlElement el, Provider prov)
        {
            id = Xml.GetAttributeString(el, "id");
            this.prov = prov;
        }

        override std::string ToString()
        {
            return id;
        }

        BufferReference createBuffer(std::string name)
        {
            return prov.createBuffer(name);
        }

        void apply();
        // should be called by our provider
        void link(Linker linker)
        {
            doLink(linker);
        }
        void bind(Binder b)
        {
            doBind(b);
        }

        IEnumerable<Provider> Dependencies
        {
            get;
        }

        void doLink(Linker user);

        void doBind(Binder bd);
    }
}
