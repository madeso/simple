using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    struct Target
    {
        void link(Linker usr);
        void apply(Action a);
        int Width { get; }
        int Height { get; }

        std::string id = String.Empty;

        std::vector<BufferReference> references = std::vector<BufferReference>();

        std::map<std::string, Size> associations = std::map<std::string, Size>();

        Provider provider = nullptr;

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

        std::string ToString() const
        {
            return Id;
        }

        BufferReference createBuffer(std::string name)
        {
            BufferReference r = BufferReference(name);
            references.Add(r);
            return r;
        }

        BufferReference createBuffer(std::string name, int width, int height)
        {
            associate(name, width, height);
            return createBuffer(name);
        }

        void associate(std::string name, int width, int height)
        {
            associations.Add(name, Size{Width = width, Height = height});
        }

        Provider Provider
        {
            get
            {
                return provider;
            }
            set
            {
                if (value == nullptr)
                    throw NullReferenceException();
                if (provider == nullptr)
                {
                    provider = value;
                }
                else
                {
                    throw std::runtime_error("failed to set " + value.ToString() + " as a provider for " + this.ToString());
                }
            }
        }

        void bind(Binder binder)
        {
            for (BufferReference br : references)
            {
                binder.reference(br);
            }
            for (KeyValuePair<std::string, Size> k : associations)
            {
                binder.associate(k.Key, k.Value);
            }
        }
    }
}
