#include "engine/fse/command.h"

#include "engine/fse/provider.h"

namespace simple::fse
{
    Command::Command(std::shared_ptr<xml::Element> el, std::shared_ptr<Provider> p)
        : prov(p)
        , id(xml::GetAttributeString(el, "id"))
    {
    }

    Command::~Command()
    {
    }

    std::string Command::ToString() const
    {
        return id;
    }

    std::shared_ptr<BufferReference> Command::CreateBuffer(std::string name)
    {
        return prov->CreateBuffer(name);
    }
}
