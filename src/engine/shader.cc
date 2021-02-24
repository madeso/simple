#include "engine/shader.h"

#include <cstdint>
#include <stdexcept>
#include <string>

#include "engine/filesystem.h"
#include "engine/opengl.h"
#include "engine/strings.h"
#include "engine/vec2.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    ShaderSource::ShaderSource(const std::string& name, const std::string& source, ShaderType type)
    {
        mShader = glCreateShader(type);
        int length = source.length();
        std::vector<char> str;
        str.resize(source.length(), 0);
        strcpy(&str[0], source.c_str());
        char* sstr = &str[0];
        glShaderSource(Shader(), 1, &sstr, &length);
        glCompileShader(mShader);

        if (false == CompileStatus())
        {
            throw std::runtime_error(fmt::format("Shader {} failed to compile: {}", name, getInfoLog()));
        }
    }

    std::string ShaderSource::getInfoLog()
    {
        int size;
        glGetShaderiv(Shader(), GL_INFO_LOG_LENGTH, &size);

        std::vector<char> log;
        log.resize(size + 1, 0);
        int length;
        glGetShaderInfoLog(Shader(), size, &length, &log[0]);

        return &log[0];
    }

    bool ShaderSource::CompileStatus()
    {
        int status;
        glGetShaderiv(Shader(), GL_COMPILE_STATUS, &status);
        return status == GL_TRUE;
    }

    int ShaderSource::Shader()
    {
        return mShader;
    }

    Uniform::Uniform(Shader* s, const std::string& name)
    {
        var = glGetUniformLocation(s->Program(), name.c_str());
        if (var == -1)
            throw std::runtime_error(fmt::format("{} is not a recognized uniform", name));
    }

    void Uniform::bindUniform(int location)
    {
        glUniform1i(var, location);
    }

    void Uniform::bindUniform(float value)
    {
        glUniform1f(var, value);
    }

    void Uniform::bindUniform(const vec2& v)
    {
        glUniform2f(var, v.x, v.y);
    }

    StaticUniformSamplerBind::StaticUniformSamplerBind(std::shared_ptr<Xml::Element> root, Shader* shader)
    {
        std::string varname = Xml::GetAttributeString(root, "id");
        location = Xml::GetAttribute<int>(
            root, "location", [](const std::string& s) -> int { return std::stoi(s); }, -1);
        var = shader->getUniform(varname);
    }

    void StaticUniformSamplerBind::bind()
    {
        var->bindUniform(location);
    }

    Shader::Shader(FileSystem* sys, const std::string& path)
    {
        if (false == IsShadersSupported())
            throw std::runtime_error("shaders not supported on your card, sorry");

        std::shared_ptr<Xml::Element> shader = Xml::Open(sys->open(path), "shader");
        LoadFrom(path, shader);
    }

    Shader::Shader(const std::string& path, std::shared_ptr<Xml::Element> shader)
    {
        if (false == IsShadersSupported())
            throw std::runtime_error("shaders not supported on your card, sorry");
        LoadFrom(path, shader);
    }

    void Shader::LoadFrom(const std::string& path, std::shared_ptr<Xml::Element> shader)
    {
        std::string vertexsource = Trim(Xml::GetTextOfSubElement(shader, "vertex"));
        std::string fragmentsource = Trim(Xml::GetTextOfSubElement(shader, "fragment"));

        mProgram = glCreateProgram();

        vertex = std::make_shared<ShaderSource>(path + " (vert)", vertexsource, ShaderSource::Vertex);
        fragment = std::make_shared<ShaderSource>(path + " (frag)", fragmentsource, ShaderSource::Fragment);

        attach(vertex);
        attach(fragment);
        auto prog = Program();
        auto func = glLinkProgram;
        glLinkProgram(prog);

        if (false == LinkStatus())
        {
            throw std::runtime_error("Link error for " + path + ": " + getInfoLog());
        }

        for (auto b : Xml::Elements(shader->GetChild("bind")))
        {
            binds.emplace_back(std::make_shared<StaticUniformSamplerBind>(b, this));
        }
    }

    std::string Shader::getInfoLog()
    {
        int size;
        glGetProgramiv(Program(), GL_INFO_LOG_LENGTH, &size);

        std::vector<char> log;
        log.resize(size + 1, 0);
        int length;
        glGetProgramInfoLog(Program(), size, &length, &log[0]);

        return &log[0];
    }

    void Shader::attach(std::shared_ptr<ShaderSource> src)
    {
        glAttachShader(Program(), src->Shader());
    }

    bool Shader::LinkStatus()
    {
        int result;
        glGetProgramiv(Program(), GL_LINK_STATUS, &result);
        return result == GL_TRUE;
    }

    int Shader::Program()
    {
        return mProgram;
    }

    bool Shader::IsShadersSupported()
    {
        bool hasVertex = true;    // IsExtensionSupported("GL_ARB_vertex_shader");
        bool hasFragment = true;  // IsExtensionSupported("GL_ARB_fragment_shader");
        return hasVertex && hasFragment;
    }

    void Shader::Bind(std::shared_ptr<Shader> shader)
    {
        glUseProgram(shader->Program());
        for (auto sb : shader->binds)
        {
            sb->bind();
        }
    }

    void Shader::Unbind()
    {
        glUseProgram(0);
    }

    std::shared_ptr<Uniform> Shader::getUniform(const std::string& varname)
    {
        return std::make_shared<Uniform>(this, varname);
    }
}
