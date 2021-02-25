#pragma once

#include <stdexcept>
#include <string>

#include "engine/opengl.h"
#include "engine/vec2.h"
#include "engine/xml.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    struct Shader;
    struct FileSystem;

    struct ShaderSource
    {
        enum ShaderType
        {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER
        };

        ShaderSource(const std::string& name, const std::string& source, ShaderType type);

        std::string GetInfoLog();

        int shader_id;

        bool GetCompileStatus();

        int GetId();
    };

    struct Uniform
    {
        int var;

        Uniform(Shader* s, const std::string& name);

        void BindUniform(int location);
        void BindUniform(float value);

        void BindUniform(const vec2& v);
    };

    struct ShaderBind
    {
        virtual ~ShaderBind() = default;
        virtual void Bind() = 0;
    };

    struct StaticUniformSamplerBind : public ShaderBind
    {
        int location;
        std::shared_ptr<Uniform> var;

        StaticUniformSamplerBind(std::shared_ptr<Xml::Element> root, Shader* shader);

        void Bind() override;
    };

    struct Shader
    {
        std::shared_ptr<ShaderSource> vertex;
        std::shared_ptr<ShaderSource> fragment;

        std::vector<std::shared_ptr<ShaderBind>> binds;

        Shader(FileSystem* sys, const std::string& path);

        Shader(const std::string& path, std::shared_ptr<Xml::Element> shader);

        void LoadFrom(const std::string& path, std::shared_ptr<Xml::Element> shader);

        std::string GetInfoLog();

        void Attach(std::shared_ptr<ShaderSource> src);

        bool GetLinkStatus();

        int GetProgramId();

        static bool IsShadersSupported();

        int program_id;

        static void Bind(std::shared_ptr<Shader> shader);

        static void Unbind();

        std::shared_ptr<Uniform> GetUniformFromName(const std::string& varname);
    };
}
