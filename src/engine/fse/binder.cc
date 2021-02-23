#include "engine/fse/binder.h"

#include <algorithm>

#include "engine/stringcounter.h"

namespace SimpleEngine::fse
{
    FboCreator::FboCreator(int width, int height)
        : pool([width, height]() { return std::make_shared<Fbo>(width, height, false); })
        , map([this](const std::string& n) { return pool.get(); })
    {
    }

    std::shared_ptr<Fbo> FboCreator::create(const std::string& name)
    {
        return map.get(name);
    }

    void FboCreator::release(std::shared_ptr<Fbo> fbo)
    {
        pool.release(fbo);
    }

    std::string Binder::ToString() const
    {
        return "<unknown binder>";
    }

    Binder::Binder(std::shared_ptr<FileSystem> sys)
        : shaders([sys](const std::string& name) { return std::make_shared<Shader>(sys.get(), name); })
    {
    }

    std::shared_ptr<Shader> Binder::getShader(const std::string& shadername)
    {
        return shaders.get(shadername);
    }

    std::shared_ptr<Shader> Binder::getShaderOrNull(const std::string& shadername)
    {
        if (shadername.empty())
        {
            return nullptr;
        }
        return shaders.get(shadername);
    }

    void Binder::reference(std::shared_ptr<BufferReference> br)
    {
        if (std::find(references.begin(), references.end(), br) != references.end())
        {
            throw std::runtime_error(fmt::format("{} already addd to {}", br->Name(), ToString()));
        }
        references.emplace_back(br);
    }

    std::shared_ptr<Fbo> Binder::allocate(const std::string& name)
    {
        return getCreator(name)->create(name);
    }

    std::shared_ptr<FboCreator> Binder::getCreator(const std::string& name)
    {
        Size size = sizeOf(name);
        auto found = creators.find(size);
        if (found == creators.end())
        {
            auto c = std::make_shared<FboCreator>(size.Width, size.Height);
            creators.emplace(size, c);
            return c;
        }
        else
        {
            return found->second;
        }
    }

    void Binder::release(std::shared_ptr<Fbo> buff)
    {
        const auto size = Size{buff->Width(), buff->Height()};
        auto found = creators.find(size);
        if (found != creators.end())
        {
            found->second->release(buff);
        }
    }

    Size Binder::sizeOf(const std::string& name)
    {
        auto found = associations.find(name);
        if (found == associations.end())
        {
            throw std::runtime_error(fmt::format("{} is missing a defined size", name));
        }
        else
        {
            return found->second;
        }
    }

    void Binder::createBuffers()
    {
        StringCounter sc;
        for (auto b : references)
        {
            sc.add(b->Name());
        }

        StringCounter usages = StringCounter();
        for (auto b : references)
        {
            auto buff = allocate(b->Name());
            b->setBuffer(buff);
            usages.add(b->Name());
            if (usages.countsOf(b->Name()) == sc.countsOf(b->Name()))
            {
                release(buff);
            }
        }
    }

    void Binder::associate(std::string p, Size size)
    {
        associations.emplace(p, size);
    }
}
