#pragma once

#include <memory>
#include <string>
#include <vector>

#include "engine/xml.h"

namespace simple::fse
{
    struct Target;
    struct RenderArgs;
    struct Command;
    struct Linker;
    struct Binder;
    struct BufferReference;

    struct Provider
    {
        Provider(std::shared_ptr<xml::Element> el);
        ~Provider();

        static void PostLoad(std::shared_ptr<Provider> provider, std::shared_ptr<xml::Element> el);

        std::shared_ptr<Target> target;
        std::string target_name;
        std::string id;

        bool autocall_commands = true;
        std::vector<std::shared_ptr<Command>> commands;
        std::vector<std::shared_ptr<Provider>> providers;

        void OnSize(int width, int height);

        const std::string& GetId() const;
        void SetId(const std::string& value);

        virtual std::string ToString() const;

        void Provide(RenderArgs* ra);

        void DenyAutocallOfCommands();

        void CallCommands();

        std::shared_ptr<BufferReference> CreateBuffer(const std::string& name);

        static void Link(std::shared_ptr<Provider> provider, Linker* linker);

        virtual void PostProvide(RenderArgs* ra) = 0;
        virtual void PostLink(Linker* linker) = 0;
        virtual void PreBind(Binder* bd) = 0;

        void Bind(Binder* bd);

        void OnLinkCompleted(Linker* linker);
    };
}
