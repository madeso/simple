#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "engine/fse/bufferreference.h"
#include "engine/xml.h"

namespace SimpleEngine::fse
{
    using namespace SimpleEngine;

    struct Linker;
    struct Provider;
    struct Binder;

    struct Target
    {
        using ApplyFunction = std::function<void()>;

        Target(std::shared_ptr<Xml::Element> el);

        virtual void link(Linker* usr) = 0;
        virtual void apply(ApplyFunction a) = 0;
        virtual int Width() = 0;
        virtual int Height() = 0;

        std::string id;
        std::vector<std::shared_ptr<BufferReference>> references;
        std::map<std::string, Size> associations;
        std::shared_ptr<Provider> provider;

        std::string Id() const;
        void Id(const std::string& value);

        virtual std::string ToString() const;

        std::shared_ptr<BufferReference> createBuffer(const std::string& name);
        std::shared_ptr<BufferReference> createBuffer(const std::string& name, int width, int height);

        void associate(const std::string& name, int width, int height);

        std::shared_ptr<Provider> Provider();
        void Provider(std::shared_ptr<fse::Provider> value);

        void bind(Binder* binder);
    };
}
