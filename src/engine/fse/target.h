#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "engine/fse/bufferreference.h"
#include "engine/xml.h"

namespace simple::fse
{
    using namespace simple;

    struct Linker;
    struct Provider;
    struct Binder;

    struct Target
    {
        using ApplyFunction = std::function<void()>;

        Target(std::shared_ptr<xml::Element> el);

        virtual void Link(Linker* usr) = 0;
        virtual void Apply(ApplyFunction a) = 0;
        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
        virtual void OnSize(int width, int height) = 0;

        std::string id;
        std::vector<std::shared_ptr<BufferReference>> references;
        std::map<std::string, Size> associations;
        std::shared_ptr<Provider> provider;

        std::string GetId() const;
        void SetId(const std::string& value);

        virtual std::string ToString() const;

        std::shared_ptr<BufferReference> CreateBuffer(const std::string& name);
        std::shared_ptr<BufferReference> CreateBuffer(const std::string& name, int width, int height);

        void SetSize(const std::string& name, int width, int height);

        std::shared_ptr<Provider> GetProvider();
        void SetProvider(std::shared_ptr<fse::Provider> value);

        void Bind(Binder* binder);
    };
}
