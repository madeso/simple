#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/xml.h"

namespace SimpleEngine::fse
{
    struct Target;
    struct RenderArgs;
    struct Command;
    struct Linker;
    struct Binder;
    struct BufferReference;

    struct Provider;

    struct Link
    {
        std::shared_ptr<Provider> prov;
        std::string name;

        Link(const std::string& n);

        void provide(RenderArgs* ra);

        void sortout(Linker* usr);
    };

    struct Provider
    {
        Provider(std::shared_ptr<Xml::Element> el);
        ~Provider();

        static void PostLoad(std::shared_ptr<Provider> provider, std::shared_ptr<Xml::Element> el);

        std::shared_ptr<Target> target;
        std::string targetname;

        std::string id;

        bool autocallCommands = true;
        std::vector<std::shared_ptr<Command>> commands;
        std::vector<std::shared_ptr<Provider>> providers;

        void OnSize(int width, int height);

        const std::string& Id() const;

        void Id(const std::string& value);

        virtual std::string ToString() const;

        void provide(RenderArgs* ra);

        void denyAutocallOfCommands();

        void callCommands();

        std::shared_ptr<BufferReference> createBuffer(const std::string& name);

        static void link(std::shared_ptr<Provider> provider, Linker* linker);

        virtual void doProvide(RenderArgs* ra) = 0;
        virtual void doLink(Linker* linker) = 0;
        virtual void doBind(Binder* bd) = 0;

        void bind(Binder* bd);

        void postlink(Linker* linker);
    };
}
