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

        std::string getInfoLog();

        int mShader;

        bool CompileStatus();

        int Shader();
    };

    struct Uniform
    {
        int var;

        Uniform(Shader* s, const std::string& name);

        void bindUniform(int location);
        void bindUniform(float value);

        void bindUniform(const vec2& v);
    };

    struct ShaderBind
    {
        virtual ~ShaderBind() = default;
        virtual void bind() = 0;
    };

    struct StaticUniformSamplerBind : public ShaderBind
    {
        int location;
        std::shared_ptr<Uniform> var;

        StaticUniformSamplerBind(std::shared_ptr<Xml::Element> root, Shader* shader);

        void bind() override;
    };

    struct Shader
    {
        std::shared_ptr<ShaderSource> vertex;
        std::shared_ptr<ShaderSource> fragment;

        std::vector<std::shared_ptr<ShaderBind>> binds;

        Shader(FileSystem* sys, const std::string& path);

        Shader(const std::string& path, std::shared_ptr<Xml::Element> shader);

        void LoadFrom(const std::string& path, std::shared_ptr<Xml::Element> shader);

        std::string getInfoLog();

        void attach(std::shared_ptr<ShaderSource> src);

        bool LinkStatus();

        int Program();

        static bool IsShadersSupported();

        int mProgram;

        static void Bind(std::shared_ptr<Shader> shader);

        static void Unbind();

        std::shared_ptr<Uniform> getUniform(const std::string& varname);
    };
}
