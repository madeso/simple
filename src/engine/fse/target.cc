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

namespace SimpleEngine::fse
{
    std::string Target::Id() const
    {
        return id;
    }

    void Target::Id(const std::string& value)
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

    std::shared_ptr<BufferReference> Target::createBuffer(const std::string& name)
    {
        auto r = std::make_shared<BufferReference>(name);
        references.emplace_back(r);
        return r;
    }

    std::shared_ptr<BufferReference> Target::createBuffer(const std::string& name, int width, int height)
    {
        associate(name, width, height);
        return createBuffer(name);
    }

    void Target::associate(const std::string& name, int width, int height)
    {
        associations.emplace(name, Size{width, height});
    }

    std::shared_ptr<Provider> Target::Provider()
    {
        return provider;
    }

    void Target::Provider(std::shared_ptr<fse::Provider> value)
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

    void Target::bind(Binder* binder)
    {
        for (auto br : references)
        {
            binder->reference(br);
        }
        for (auto k : associations)
        {
            binder->associate(k.first, k.second);
        }
    }
}
