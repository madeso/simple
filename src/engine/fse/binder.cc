#include "engine/fse/binder.h"

#include <algorithm>

#include "engine/stringcounter.h"

namespace simple::fse
{
    FboCreator::FboCreator(int width, int height)
        : pool([width, height]() { return std::make_shared<Fbo>(width, height, false); })
        , map([this](const std::string& n) { return pool.Create(); })
    {
    }

    std::shared_ptr<Fbo> FboCreator::create(const std::string& name)
    {
        return map.GetOrCreate(name);
    }

    void FboCreator::Destroy(std::shared_ptr<Fbo> fbo)
    {
        pool.Destroy(fbo);
    }

    std::string Binder::ToString() const
    {
        return "<unknown binder>";
    }

    Binder::Binder(std::shared_ptr<FileSystem> sys)
        : shaders([sys](const std::string& name) { return std::make_shared<ShaderProgram>(sys.get(), name); })
    {
    }

    std::shared_ptr<ShaderProgram> Binder::GetShader(const std::string& shadername)
    {
        return shaders.GetOrCreate(shadername);
    }

    std::shared_ptr<ShaderProgram> Binder::GetShaderOrNull(const std::string& shadername)
    {
        if (shadername.empty())
        {
            return nullptr;
        }
        return shaders.GetOrCreate(shadername);
    }

    void Binder::AddReference(std::shared_ptr<BufferReference> br)
    {
        if (std::find(references.begin(), references.end(), br) != references.end())
        {
            throw std::runtime_error(fmt::format("{} already addd to {}", br->name, ToString()));
        }
        references.emplace_back(br);
    }

    std::shared_ptr<Fbo> Binder::CreateFbo(const std::string& name)
    {
        return GetCreator(name)->create(name);
    }

    std::shared_ptr<FboCreator> Binder::GetCreator(const std::string& name)
    {
        Size size = GetSize(name);
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

    void Binder::DestroyFbo(std::shared_ptr<Fbo> buff)
    {
        const auto size = Size{buff->Width(), buff->Height()};
        auto found = creators.find(size);
        if (found != creators.end())
        {
            found->second->Destroy(buff);
        }
    }

    Size Binder::GetSize(const std::string& name)
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

    void Binder::CreateBuffers()
    {
        StringCounter sc;
        for (auto b : references)
        {
            sc.Add(b->name);
        }

        StringCounter usages = StringCounter();
        for (auto b : references)
        {
            auto buff = CreateFbo(b->name);
            b->SetBuffer(buff);
            usages.Add(b->name);
            if (usages.GetCountsOf(b->name) == sc.GetCountsOf(b->name))
            {
                DestroyFbo(buff);
            }
        }
    }

    void Binder::SetSize(std::string p, Size size)
    {
        associations.emplace(p, size);
    }
}
