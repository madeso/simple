using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse
{
    public class Linker
    {
        Map<Provider> providers = new Map<Provider>(delegate(string name) { throw new Exception(name + " is not a defined provider"); });
        Map<Target> targets = new Map<Target>(delegate(string name) { throw new Exception(name + " is not a defined target"); });

        public Provider getProvider(string name)
        {
            return providers.get(name);
        }
        public void addProvider(Provider prov)
        {
            providers.add(prov.Id, prov);
        }

        public void addTarget(Target targ)
        {
            targets.add(targ.Id, targ);
        }
        public Target getTarget(string name)
        {
            return targets.get(name);
        }

        public Pipeline getPipeline(Target target)
        {
            Pipeline pl = new Pipeline();

            pl.add( target.Provider );

            return pl;
        }

        public void link()
        {
            foreach(Provider p in providers.Data)
            {
                p.link(this);
            }

            foreach (Target t in targets.Data)
            {
                t.link(this);
                if (t.Provider == null) throw new Exception(t.ToString() + " is missing a provider");
            }

            foreach (Provider p in providers.Data)
            {
                p.postlink(this);
            }
        }

        internal string read(string path, MediaLoader ml, int width, int height)
        {
            XmlElement root = Xml.Open(Xml.FromStream(ml.FS.open(path)), "pipeline");
            string t = Xml.GetAttributeString(root, "target");
            foreach (XmlElement targetElement in Xml.Elements(root["targets"]))
            {
                Target target = Targets.Targets.Create(targetElement, width, height);
                target.Id = Xml.GetAttributeString(targetElement, "id");
                addTarget(target);
            }
            foreach (XmlElement providerElement in Xml.Elements(root["providers"]))
            {
                Provider provider = Providers.Providers.Create(providerElement);
                provider.Id = Xml.GetAttributeString(providerElement, "id");
                addProvider(provider);
            }

            return t;
        }
    }
}
