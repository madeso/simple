#include "engine/fse/command.bindbuffer.h"

#include "engine/fse/binder.h"
#include "engine/fse/bufferreference.h"
#include "engine/fse/linker.h"
#include "engine/fse/target.h"
#include "fmt/core.h"

namespace simple::fse::commands
{
    std::string BindBufferCommand::ToString() const
    {
        return fmt::format("{} binds buffer {} to {}", Command::ToString(), name, location);
    }

    BindBufferCommand::BindBufferCommand(std::shared_ptr<xml::Element> el, std::shared_ptr<Provider> prov)
        : Command(el, prov)
        , name(xml::GetAttributeString(el, "buffer"))
        , location(xml::GetAttribute<int>(
              el, "location", [](const std::string& s) { return std::stoi(s); }, -1))
    {
    }

    void BindBufferCommand::Apply()
    {
        buffer->BindTexture(location);
    }

    void BindBufferCommand::Link(Linker* user)
    {
        buffer = CreateBuffer(name);
        target = user->GetTarget(name);
    }

    void BindBufferCommand::Bind(Binder* bd)
    {
        bd->AddReference(buffer);
    }

    std::vector<std::shared_ptr<Provider>> BindBufferCommand::GetDependencies()
    {
        return {target->GetProvider()};
    }
}
