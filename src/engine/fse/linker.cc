using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse
{
    struct Linker
    {
        Map<Provider> providers = Map<Provider>(delegate(std::string name) { throw Exception(name + " is not a defined provider"); });
        Map<Target> targets = Map<Target>(delegate(std::string name) { throw Exception(name + " is not a defined target"); });

        Provider getProvider(std::string name)
        {
            return providers.get(name);
        }
        void addProvider(Provider prov)
        {
            providers.add(prov.Id, prov);
        }

        void addTarget(Target targ)
        {
            targets.add(targ.Id, targ);
        }
        Target getTarget(std::string name)
        {
            return targets.get(name);
        }

        Pipeline getPipeline(Target target)
        {
            Pipeline pl = Pipeline();

            pl.add(target.Provider);

            return pl;
        }

        void link()
        {
            for (Provider p : providers.Data)
            {
                p.link(this);
            }

            for (Target t : targets.Data)
            {
                t.link(this);
                if (t.Provider == nullptr)
                    throw Exception(t.ToString() + " is missing a provider");
            }

            for (Provider p : providers.Data)
            {
                p.postlink(this);
            }
        }

        std::string read(std::string path, MediaLoader ml, int width, int height)
        {
            XmlElement root = Xml.Open(Xml.FromStream(ml.FS.open(path)), "pipeline");
            std::string t = Xml.GetAttributeString(root, "target");
            for (XmlElement targetElement : Xml.Elements(root["targets"]))
            {
                Target target = Targets.Targets.Create(targetElement, width, height);
                target.Id = Xml.GetAttributeString(targetElement, "id");
                addTarget(target);
            }
            for (XmlElement providerElement : Xml.Elements(root["providers"]))
            {
                Provider provider = Providers.Providers.Create(providerElement);
                provider.Id = Xml.GetAttributeString(providerElement, "id");
                addProvider(provider);
            }

            return t;
        }
    }
}
