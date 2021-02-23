#include <memory>
#include <stdexcept>
#include <string>

#include "engine/stringseperator.h"
#include "fmt/core.h"

namespace SimpleEngine::fse
{
    struct Target;

    struct Provider
    {
        std::shared_ptr<Target> target;
        std::string targetname;

        std::string id;

        bool autocallCommands = true;
        std::vector<std::shared_ptr<Command>> commands;
        std::vector<std::shared_ptr<Provider>> providers;

        const std::string& Id() const
        {
            return id;
        }

        void Id(const std::string& value)
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

        std::string ToString() const
        {
            return Id() + "(" + targetname + "): <" + StringSeperator(",", " and ", "").ToString(commands) + ">";
        }

        void provide(RenderArgs ra)
        {
            if (autocallCommands)
            {
                callCommands();
            }

            target->apply([this, ra]() {
                doProvide(ra);
            });
        }

        void denyAutocallOfCommands()
        {
            autocallCommands = false;
        }

        void callCommands()
        {
            for (auto c : commands)
            {
                c->apply();
            }
        }

        void doProvide(RenderArgs ra);

        Target Target
        {
            get
            {
                return target;
            }
        }

        struct Link
        {
            Provider prov = nullptr;
            std::string name;

            Link(std::string name)
            {
                this.name = name;
            }

            void provide(RenderArgs ra)
            {
                prov.doProvide(ra);
            }

            void sortout(Linker usr)
            {
                prov = usr.getProvider(name);
            }
        }

        BufferReference
        createBuffer(std::string name)
        {
            return BufferReference(name);
        }

        Provider(std::shared_ptr<Xml::Element> el)
        {
            targetname = Xml::GetAttributeString(el, "target");

            for (std::shared_ptr<Xml::Element> e : Xml::Elements(el))
            {
                commands.Add(Commands.Commands.Create(e, this));
            }
        }

        void link(Linker linker)
        {
            if (false == std::string.IsNullOrEmpty(targetname))
            {
                target = linker.getTarget(targetname);
                target.Provider = this;
            }

            for (auto c : commands)
            {
                c->link(linker);
            }

            doLink(linker);
        }

        void doLink(Linker linker);

        void bind(Binder bd)
        {
            doBind(bd);

            for (auto c : commands)
            {
                c->bind(bd);
            }
        }

        void doBind(Binder bd);

        void postlink(Linker linker)
        {
            for (auto c : commands)
            {
                c->link(linker);

                for (auto p : c.Dependencies)
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
}
