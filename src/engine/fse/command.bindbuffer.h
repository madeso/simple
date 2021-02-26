#pragma once

#include <memory>
#include <string>

#include "engine/fse/command.h"

namespace simple::fse
{
    struct Binder;
    struct BufferReference;
    struct Linker;
    struct Target;
}

namespace simple::fse::commands
{
    struct BindBufferCommand : public Command
    {
        std::shared_ptr<BufferReference> buffer;
        std::shared_ptr<Target> target;
        int location;
        std::string name;

        std::string ToString() const override;

        BindBufferCommand(std::shared_ptr<xml::Element> el, std::shared_ptr<Provider> prov);

        void Apply() override;

        void Link(Linker* user) override;

        void Bind(Binder* bd) override;

        std::vector<std::shared_ptr<Provider>> GetDependencies() override;
    };
}
