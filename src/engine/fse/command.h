#pragma once

#include <memory>
#include <string>

#include "engine/xml.h"

namespace simple::fse
{
    struct Provider;
    struct Linker;
    struct BufferReference;
    struct Binder;

    struct Command
    {
        std::shared_ptr<Provider> prov;
        std::string id;

        Command(std::shared_ptr<xml::Element> el, std::shared_ptr<Provider> p);

        virtual ~Command();

        virtual std::string ToString() const;

        std::shared_ptr<BufferReference> CreateBuffer(std::string name);

        virtual void Apply() = 0;
        // should be called by our provider

        virtual std::vector<std::shared_ptr<Provider>> GetDependencies() = 0;

        virtual void Link(Linker* user) = 0;

        virtual void Bind(Binder* bd) = 0;
    };
}
