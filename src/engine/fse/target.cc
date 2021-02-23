#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "engine/fbo.h"
#include "engine/fse/bufferreference.h"
#include "engine/shader.h"
#include "fmt/core.h"

namespace SimpleEngine::fse
{
    using namespace SimpleEngine;

    struct Target
    {
        virtual void link(Linker usr) = 0;
        virtual void apply(std::function<void()> a) = 0;
        virtual int Width() = 0;
        virtual int Height() = 0;

        std::string id;

        std::vector<std::shared_ptr<BufferReference>> references;

        std::map<std::string, Size> associations;

        std::shared_ptr<Provider> provider;

        std::string Id() const
        {
            return id;
        }

        void Id(const std::string& value)
        {
            if (id.empty())
                id = value;
            else
                throw std::runtime_error("Unable to change id from " + id + " to " + value);
        }

        const std::string& ToString() const
        {
            return Id();
        }

        std::shared_ptr<BufferReference> createBuffer(const std::string& name)
        {
            auto r = std::make_shared<BufferReference>(name);
            references.emplace_back(r);
            return r;
        }

        std::shared_ptr<BufferReference> createBuffer(const std::string& name, int width, int height)
        {
            associate(name, width, height);
            return createBuffer(name);
        }

        void associate(const std::string& name, int width, int height)
        {
            associations.emplace(name, Size{width, height});
        }

        std::shared_ptr<Provider> Provider()
        {
            return provider;
        }

        void Provider(std::shared_ptr<Provider> value)
        {
            if (value == nullptr)
                throw std::runtime_error("Provider is null");

            if (provider == nullptr)
            {
                provider = value;
            }
            else
            {
                throw std::runtime_error(fmt::format("failed to set {} as a provider for {}", value->ToString(), ToString()));
            }
        }

        void bind(Binder binder)
        {
            for (auto br : references)
            {
                binder.reference(br);
            }
            for (auto k : associations)
            {
                binder.associate(k.first, k.second);
            }
        }
    }
}
