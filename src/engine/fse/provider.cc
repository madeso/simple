using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse
{
    struct Provider
    {
        Target target;
        std::string targetname;

        std::string id = String.Empty;

        std::string Id
        {
            get
            {
                return id;
            }
            set
            {
                if (std::string.IsNullOrEmpty(id))
                    id = value;
                else
                    throw std::runtime_error("Unable to change id from " + id + " to " + value);
            }
        }

        override std::string ToString()
        {
            return Id + "(" + targetname + "): <" + StringSeperator(",", " and ", "").Append(commands.ToArray()).ToString() + ">";
        }

        bool autocallCommands = true;

        void provide(RenderArgs ra)
        {
            if (autocallCommands)
            {
                callCommands();
            }

            target.apply(delegate {
                doProvide(ra);
            });
        }

        void denyAutocallOfCommands()
        {
            autocallCommands = false;
        }

        void callCommands()
        {
            for (Command c : commands)
            {
                c.apply();
            }
        }

        void doProvide(RenderArgs ra);

        Target Target
        {
            get
            {
                return target;
            }
        }

        struct Link
        {
            Provider prov = nullptr;
            std::string name;

            Link(std::string name)
            {
                this.name = name;
            }

            void provide(RenderArgs ra)
            {
                prov.doProvide(ra);
            }

            void sortout(Linker usr)
            {
                prov = usr.getProvider(name);
            }
        }

        BufferReference
        createBuffer(std::string name)
        {
            return BufferReference(name);
        }

        Provider(XmlElement el)
        {
            targetname = Xml.GetAttributeString(el, "target");

            for (XmlElement e : Xml.Elements(el))
            {
                commands.Add(Commands.Commands.Create(e, this));
            }
        }

        std::vector<Command> commands = std::vector<Command>();

        std::vector<Provider> providers = std::vector<Provider>();

        IEnumerable<Provider> Providers
        {
            get
            {
                return providers;
            }
        }

        void link(Linker linker)
        {
            if (false == std::string.IsNullOrEmpty(targetname))
            {
                target = linker.getTarget(targetname);
                target.Provider = this;
            }

            for (Command c : commands)
            {
                c.link(linker);
            }

            doLink(linker);
        }

        void doLink(Linker linker);

        void bind(Binder bd)
        {
            doBind(bd);

            for (Command c : commands)
            {
                c.bind(bd);
            }
        }

        void doBind(Binder bd);

        void postlink(Linker linker)
        {
            for (Command c : commands)
            {
                c.link(linker);

                for (Provider p : c.Dependencies)
                {
                    if (p == nullptr)
                        throw NullReferenceException();
                    this.providers.Add(p);
                }
            }
        }
    }
}
