#include "engine/fse/provider.h"

#include <stdexcept>

#include "engine/fse/command.commands.h"
#include "engine/fse/command.h"
#include "engine/fse/linker.h"
#include "engine/fse/renderargs.h"
#include "engine/fse/target.h"
#include "engine/stringseperator.h"
#include "fmt/core.h"

namespace simple::fse
{
    const std::string& Provider::GetId() const
    {
        return id;
    }

    void Provider::OnSize(int width, int height)
    {
        target->OnSize(width, height);
    }

    Provider::Provider(std::shared_ptr<xml::Element> el)
        : target_name(xml::GetAttributeString(el, "target"))
        , id(xml::GetAttributeString(el, "id"))
    {
    }

    void Provider::PostLoad(std::shared_ptr<Provider> provider, std::shared_ptr<xml::Element> el)
    {
        for (std::shared_ptr<xml::Element> e : xml::Elements(el))
        {
            provider->commands.emplace_back(commands::Create(e, provider));
        }
    }

    Provider::~Provider()
    {
    }

    void Provider::SetId(const std::string& value)
    {
        if (id.empty())
        {
            id = value;
        }
        else
        {
            throw std::runtime_error(fmt::format("Unable to change id from {} to {}", id, value));
        }
    }

    std::string Provider::ToString() const
    {
        std::vector<std::string> command_names;
        for (auto& c : commands)
        {
            command_names.emplace_back(c->ToString());
        }
        const auto command_string = StringSeperator(",", " and ", "").ToString(command_names);
        return fmt::format("{}({}): <{}>", GetId(), target_name, command_string);
    }

    void Provider::Provide(RenderArgs* ra)
    {
        if (autocall_commands)
        {
            CallCommands();
        }

        target->Apply([this, ra]() { PostProvide(ra); });
    }

    void Provider::DenyAutocallOfCommands()
    {
        autocall_commands = false;
    }

    void Provider::CallCommands()
    {
        for (auto c : commands)
        {
            c->Apply();
        }
    }

    std::shared_ptr<BufferReference> Provider::CreateBuffer(const std::string& name)
    {
        return std::make_shared<BufferReference>(name);
    }

    void Provider::Link(std::shared_ptr<Provider> provider, Linker* linker)
    {
        if (provider->target_name != "")
        {
            provider->target = linker->GetTarget(provider->target_name);
            provider->target->SetProvider(provider);
        }

        for (auto c : provider->commands)
        {
            c->Link(linker);
        }

        provider->PostLink(linker);
    }

    void Provider::Bind(Binder* bd)
    {
        PreBind(bd);

        for (auto c : commands)
        {
            c->Bind(bd);
        }
    }

    void Provider::OnLinkCompleted(Linker* linker)
    {
        for (auto c : commands)
        {
            // why call link twice?
            c->Link(linker);

            for (auto p : c->GetDependencies())
            {
                if (p == nullptr)
                {
                    throw std::runtime_error("null pointer exception");
                }
                providers.emplace_back(p);
            }
        }
    }
}
