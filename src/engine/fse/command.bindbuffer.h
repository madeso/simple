#pragma once

#include <memory>
#include <string>

#include "engine/fse/command.h"

namespace SimpleEngine::fse
{
    struct Binder;
    struct BufferReference;
    struct Linker;
    struct Target;
}

namespace SimpleEngine::fse::Commands
{
    struct BindBufferCommand : public Command
    {
        std::shared_ptr<BufferReference> buffer;
        std::shared_ptr<Target> targ;
        int location;
        std::string name;

        std::string ToString() const override;

        BindBufferCommand(std::shared_ptr<Xml::Element> el, std::shared_ptr<Provider> prov);

        void apply() override;

        void doLink(Linker* user) override;

        void doBind(Binder* bd) override;

        std::vector<std::shared_ptr<Provider>> Dependencies() override;
    };
}
