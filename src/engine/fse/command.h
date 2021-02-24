#pragma once

#include <memory>
#include <string>

#include "engine/xml.h"

namespace SimpleEngine::fse
{
    struct Provider;
    struct Linker;
    struct BufferReference;
    struct Binder;

    struct Command
    {
        std::shared_ptr<Provider> prov;
        std::string id;

        Command(std::shared_ptr<Xml::Element> el, std::shared_ptr<Provider> p);

        ~Command();

        virtual std::string ToString() const;

        std::shared_ptr<BufferReference> createBuffer(std::string name);

        virtual void apply() = 0;
        // should be called by our provider

        void link(Linker* linker);

        void bind(Binder* b);

        virtual std::vector<std::shared_ptr<Provider>> Dependencies() = 0;

        virtual void doLink(Linker* user) = 0;

        virtual void doBind(Binder* bd) = 0;
    };
}
