using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    public abstract class Target
    {
        public abstract void link(Linker usr);
        public abstract void apply(Action a);
        public abstract int Width { get; }
        public abstract int Height { get; }

        private string id = String.Empty;
        private List<BufferReference> references = new List<BufferReference>();
        private Dictionary<string, Size> associations = new Dictionary<string, Size>();
        private Provider provider = null;

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
            return Id;
        }

        protected BufferReference createBuffer(string name)
        {
            BufferReference r = new BufferReference(name);
            references.Add(r);
            return r;
        }

        protected BufferReference createBuffer(string name, int width, int height)
        {
            associate(name, width, height);
            return createBuffer(name);
        }

        private void associate(string name, int width, int height)
        {
            associations.Add(name, new Size { Width = width, Height = height });
        }

        
        public Provider Provider
        {
            get
            {
                return provider;
            }
            set
            {
                if (value == null) throw new NullReferenceException();
                if (provider == null)
                {
                    provider = value;
                }
                else
                {
                    throw new Exception("failed to set " + value.ToString() + " as a provider for " + this.ToString());
                }
            }
        }

        internal void bind(Binder binder)
        {
            foreach (BufferReference br in references)
            {
                binder.reference(br);
            }
            foreach (KeyValuePair<string, Size> k in associations)
            {
                binder.associate(k.Key, k.Value);
            }
        }
    }
}
