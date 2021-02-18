using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine.fse
{
    public abstract class Provider
    {
        Target target;
        readonly string targetname;

        private string id = String.Empty;

        public string Id
        {
            get
            {
                return id;
            }
            set
            {
                if (string.IsNullOrEmpty(id)) id = value;
                else throw new Exception("Unable to change id from " + id + " to " + value);
            }
        }

        public override string ToString()
        {
            return Id + "(" + targetname + "): <" + new StringSeperator(",", " and ", "").Append(commands.ToArray()).ToString() + ">";
        }

        private bool autocallCommands = true;

        public void provide(RenderArgs ra)
        {
            if (autocallCommands)
            {
                callCommands();
            }

            target.apply(delegate
            {
                doProvide(ra);
            });
        }


        protected void denyAutocallOfCommands()
        {
            autocallCommands = false;
        }

        protected void callCommands()
        {
            foreach (Command c in commands)
            {
                c.apply();
            }
        }
        protected abstract void doProvide(RenderArgs ra);

        internal Target Target
        {
            get
            {
                return target;
            }
        }

        public class Link
        {
            Provider prov = null;
            readonly string name;

            public Link(string name)
            {
                this.name = name;
            }

            public void provide(RenderArgs ra)
            {
                prov.doProvide(ra);
            }

            public void sortout(Linker usr)
            {
                prov = usr.getProvider(name);
            }
        }

        internal BufferReference createBuffer(string name)
        {
            return new BufferReference(name);
        }

        protected Provider(XmlElement el)
        {
            targetname = Xml.GetAttributeString(el, "target");

            foreach (XmlElement e in Xml.Elements(el))
            {
                commands.Add(Commands.Commands.Create(e, this));
            }
        }

        private List<Command> commands = new List<Command>();
        private List<Provider> providers = new List<Provider>();

        public IEnumerable<Provider> Providers
        {
            get
            {
                return providers;
            }
        }

        public void link(Linker linker)
        {
            if (false == string.IsNullOrEmpty(targetname))
            {
                target = linker.getTarget(targetname);
                target.Provider = this;
            }

            foreach (Command c in commands)
            {
                c.link(linker);
            }

            doLink(linker);
        }

        protected abstract void doLink(Linker linker);

        public void bind(Binder bd)
        {
            doBind(bd);

            foreach (Command c in commands)
            {
                c.bind(bd);
            }
        }
        protected abstract void doBind(Binder bd);

        internal void postlink(Linker linker)
        {
            foreach (Command c in commands)
            {
                c.link(linker);

                foreach (Provider p in c.Dependencies)
                {
                    if (p == null) throw new NullReferenceException();
                    this.providers.Add(p);
                }
            }
        }
    }
}
