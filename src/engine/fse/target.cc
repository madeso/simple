#include "engine/fse/target.h"

#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "engine/fbo.h"
#include "engine/fse/binder.h"
#include "engine/fse/bufferreference.h"
#include "engine/fse/provider.h"
#include "engine/shader.h"
#include "fmt/core.h"

namespace simple::fse
{
    Target::Target(std::shared_ptr<xml::Element> el)
        : id(xml::GetAttributeString(el, "id"))
    {
    }

    std::string Target::GetId() const
    {
        return id;
    }

    void Target::SetId(const std::string& value)
    {
        if (id.empty())
            id = value;
        else
            throw std::runtime_error("Unable to change id from " + id + " to " + value);
    }

    std::string Target::ToString() const
    {
        return id;
    }

    std::shared_ptr<BufferReference> Target::CreateBuffer(const std::string& name)
    {
        auto r = std::make_shared<BufferReference>(name);
        references.emplace_back(r);
        return r;
    }

    std::shared_ptr<BufferReference> Target::CreateBuffer(const std::string& name, int width, int height)
    {
        SetSize(name, width, height);
        return CreateBuffer(name);
    }

    void Target::SetSize(const std::string& name, int width, int height)
    {
        associations.emplace(name, Size{width, height});
    }

    std::shared_ptr<Provider> Target::GetProvider()
    {
        return provider;
    }

    void Target::SetProvider(std::shared_ptr<fse::Provider> value)
    {
        if (value == nullptr)
            throw std::runtime_error("Provider is null");

        if (provider == nullptr)
        {
            provider = value;
        }
        else
        {
            throw std::runtime_error(
                fmt::format("failed to set {} as a provider for {}", value->ToString(), ToString()));
        }
    }

    void Target::Bind(Binder* binder)
    {
        for (auto br : references)
        {
            binder->AddReference(br);
        }
        for (auto k : associations)
        {
            binder->SetSize(k.first, k.second);
        }
    }
}
