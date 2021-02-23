#include <memory>
#include <string>

#include "engine/xml.h"

namespace SimpleEngine::fse
{
    struct Provider;
    struct Linker;
    struct BufferReference;
    struct Binder;

    struct Command
    {
        std::shared_ptr<Provider> prov;
        std::string id;

        Command(std::shared_ptr<Xml::Element> el, std::shared_ptr<Provider> p)
            : prov(prov)
            , id(Xml::GetAttributeString(el, "id"))
        {
        }

        std::string ToString() const
        {
            return id;
        }

        std::shared_ptr<BufferReference> createBuffer(std::string name)
        {
            return prov->createBuffer(name);
        }

        virtual void apply() = 0;
        // should be called by our provider

        void link(Linker* linker)
        {
            doLink(linker);
        }

        void bind(Binder* b)
        {
            doBind(b);
        }

        virtual std::vector<std::shared_ptr<Provider>> Dependencies() = 0;

        virtual void doLink(Linker* user) = 0;

        virtual void doBind(Binder* bd) = 0;
    };
}
