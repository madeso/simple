#include "engine/fse/command.h"

#include "engine/fse/provider.h"

namespace SimpleEngine::fse
{
    Command::Command(std::shared_ptr<Xml::Element> el, std::shared_ptr<Provider> p)
        : prov(p)
        , id(Xml::GetAttributeString(el, "id"))
    {
    }

    Command::~Command()
    {
    }

    std::string Command::ToString() const
    {
        return id;
    }

    std::shared_ptr<BufferReference> Command::createBuffer(std::string name)
    {
        return prov->createBuffer(name);
    }

    void Command::link(Linker* linker)
    {
        doLink(linker);
    }

    void Command::bind(Binder* b)
    {
        doBind(b);
    }
}
