#include "engine/fse/provider.h"

#include <stdexcept>

#include "engine/fse/command.commands.h"
#include "engine/fse/command.h"
#include "engine/fse/linker.h"
#include "engine/fse/renderargs.h"
#include "engine/fse/target.h"
#include "engine/stringseperator.h"
#include "fmt/core.h"

namespace SimpleEngine::fse
{
    Link::Link(const std::string& n)
        : name(n)
    {
    }

    void Link::provide(RenderArgs* ra)
    {
        prov->doProvide(ra);
    }

    void Link::sortout(Linker* usr)
    {
        prov = usr->getProvider(name);
    }

    const std::string& Provider::Id() const
    {
        return id;
    }

    void Provider::OnSize(int width, int height)
    {
        target->OnSize(width, height);
    }

    Provider::Provider(std::shared_ptr<Xml::Element> el)
        : targetname(Xml::GetAttributeString(el, "target"))
        , id(Xml::GetAttributeString(el, "id"))
    {
    }

    void Provider::PostLoad(std::shared_ptr<Provider> provider, std::shared_ptr<Xml::Element> el)
    {
        for (std::shared_ptr<Xml::Element> e : Xml::Elements(el))
        {
            provider->commands.emplace_back(Commands::Create(e, provider));
        }
    }

    Provider::~Provider()
    {
    }

    void Provider::Id(const std::string& value)
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
        return fmt::format("{}({}): <{}>", Id(), targetname, command_string);
    }

    void Provider::provide(RenderArgs* ra)
    {
        if (autocallCommands)
        {
            callCommands();
        }

        target->apply([this, ra]() { doProvide(ra); });
    }

    void Provider::denyAutocallOfCommands()
    {
        autocallCommands = false;
    }

    void Provider::callCommands()
    {
        for (auto c : commands)
        {
            c->apply();
        }
    }

    std::shared_ptr<BufferReference> Provider::createBuffer(const std::string& name)
    {
        return std::make_shared<BufferReference>(name);
    }

    void Provider::link(std::shared_ptr<Provider> provider, Linker* linker)
    {
        if (provider->targetname != "")
        {
            provider->target = linker->getTarget(provider->targetname);
            provider->target->Provider(provider);
        }

        for (auto c : provider->commands)
        {
            c->link(linker);
        }

        provider->doLink(linker);
    }

    void Provider::bind(Binder* bd)
    {
        doBind(bd);

        for (auto c : commands)
        {
            c->bind(bd);
        }
    }

    void Provider::postlink(Linker* linker)
    {
        for (auto c : commands)
        {
            c->link(linker);

            for (auto p : c->Dependencies())
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
