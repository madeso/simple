#pragma once

#include <stdexcept>
#include <string>

#include "engine/opengl.h"
#include "engine/vec2.h"
#include "engine/xml.h"
#include "fmt/core.h"

namespace simple
{
    struct ShaderProgram;
    struct FileSystem;

    /// @addtogroup shader
    /// @{
    struct Shader
    {
        enum ShaderType
        {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER
        };

        Shader(const std::string& name, const std::string& source, ShaderType type);

        std::string GetInfoLog();

        int id;

        bool GetCompileStatus();
    };

    struct Uniform
    {
        int id;

        Uniform(ShaderProgram* program, const std::string& name);

        void SetTexture(int location);
        void Set(float value);

        void Set(const vec2& value);
    };

    struct ShaderBind
    {
        virtual ~ShaderBind() = default;
        virtual void Bind() = 0;
    };

    struct StaticUniformSamplerBind : public ShaderBind
    {
        int location;
        std::shared_ptr<Uniform> uniform;

        StaticUniformSamplerBind(std::shared_ptr<xml::Element> root, ShaderProgram* program);

        void Bind() override;
    };

    struct ShaderProgram
    {
        std::shared_ptr<Shader> vertex_shader;
        std::shared_ptr<Shader> fragment_shader;

        std::vector<std::shared_ptr<ShaderBind>> binds;

        ShaderProgram(FileSystem* fs, const std::string& path);

        ShaderProgram(const std::string& path, std::shared_ptr<xml::Element> shader_root);

        void LoadFrom(const std::string& path, std::shared_ptr<xml::Element> shader_root);

        std::string GetInfoLog() const;

        void Attach(std::shared_ptr<Shader> shader);

        bool GetLinkStatus();

        int id;

        static void Bind(std::shared_ptr<ShaderProgram> program);

        static void Unbind();

        std::shared_ptr<Uniform> GetUniformFromName(const std::string& name);
    };
    /// @}
}
