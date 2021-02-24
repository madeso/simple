#include "engine/fse/command.bindbuffer.h"

#include "engine/fse/binder.h"
#include "engine/fse/bufferreference.h"
#include "engine/fse/linker.h"
#include "engine/fse/target.h"
#include "fmt/core.h"

namespace SimpleEngine::fse::Commands
{
    std::string BindBufferCommand::ToString() const
    {
        return fmt::format("{} binds buffer {} to {}", Command::ToString(), name, location);
    }

    BindBufferCommand::BindBufferCommand(std::shared_ptr<Xml::Element> el, std::shared_ptr<Provider> prov)
        : Command(el, prov)
        , name(Xml::GetAttributeString(el, "buffer"))
        , location(Xml::GetAttribute<int>(
              el, "location", [](const std::string& s) { return std::stoi(s); }, -1))
    {
    }

    void BindBufferCommand::apply()
    {
        buffer->bindTexture(location);
    }

    void BindBufferCommand::doLink(Linker* user)
    {
        buffer = createBuffer(name);
        targ = user->getTarget(name);
    }

    void BindBufferCommand::doBind(Binder* bd)
    {
        bd->reference(buffer);
    }

    std::vector<std::shared_ptr<Provider>> BindBufferCommand::Dependencies()
    {
        return {targ->Provider()};
    }
}
