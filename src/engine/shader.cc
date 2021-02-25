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
        shader_id = glCreateShader(type);
        int length = source.length();
        std::vector<char> str;
        str.resize(source.length()+1, 0);
        strcpy(str.data(), source.c_str());
        char* sstr = str.data();
        glShaderSource(GetId(), 1, &sstr, &length);
        glCompileShader(shader_id);

        if (false == GetCompileStatus())
        {
            throw std::runtime_error(fmt::format("Shader {} failed to compile: {}", name, GetInfoLog()));
        }
    }

    std::string ShaderSource::GetInfoLog()
    {
        int size;
        glGetShaderiv(GetId(), GL_INFO_LOG_LENGTH, &size);

        std::vector<char> log;
        log.resize(size + 1, 0);
        int length;
        glGetShaderInfoLog(GetId(), size, &length, &log[0]);

        return &log[0];
    }

    bool ShaderSource::GetCompileStatus()
    {
        int status;
        glGetShaderiv(GetId(), GL_COMPILE_STATUS, &status);
        return status == GL_TRUE;
    }

    int ShaderSource::GetId()
    {
        return shader_id;
    }

    Uniform::Uniform(Shader* s, const std::string& name)
    {
        var = glGetUniformLocation(s->GetProgramId(), name.c_str());
        if (var == -1)
            throw std::runtime_error(fmt::format("{} is not a recognized uniform", name));
    }

    void Uniform::BindUniform(int location)
    {
        glUniform1i(var, location);
    }

    void Uniform::BindUniform(float value)
    {
        glUniform1f(var, value);
    }

    void Uniform::BindUniform(const vec2& v)
    {
        glUniform2f(var, v.x, v.y);
    }

    StaticUniformSamplerBind::StaticUniformSamplerBind(std::shared_ptr<Xml::Element> root, Shader* shader)
    {
        std::string varname = Xml::GetAttributeString(root, "id");
        location = Xml::GetAttribute<int>(
            root, "location", [](const std::string& s) -> int { return std::stoi(s); }, -1);
        var = shader->GetUniformFromName(varname);
    }

    void StaticUniformSamplerBind::Bind()
    {
        var->BindUniform(location);
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

        program_id = glCreateProgram();

        vertex = std::make_shared<ShaderSource>(path + " (vert)", vertexsource, ShaderSource::Vertex);
        fragment = std::make_shared<ShaderSource>(path + " (frag)", fragmentsource, ShaderSource::Fragment);

        Attach(vertex);
        Attach(fragment);
        auto prog = GetProgramId();
        auto func = glLinkProgram;
        glLinkProgram(prog);

        if (false == GetLinkStatus())
        {
            throw std::runtime_error("Link error for " + path + ": " + GetInfoLog());
        }

        for (auto b : Xml::Elements(shader->GetChild("bind")))
        {
            binds.emplace_back(std::make_shared<StaticUniformSamplerBind>(b, this));
        }
    }

    std::string Shader::GetInfoLog()
    {
        int size;
        glGetProgramiv(GetProgramId(), GL_INFO_LOG_LENGTH, &size);

        std::vector<char> log;
        log.resize(size + 1, 0);
        int length;
        glGetProgramInfoLog(GetProgramId(), size, &length, &log[0]);

        return &log[0];
    }

    void Shader::Attach(std::shared_ptr<ShaderSource> src)
    {
        glAttachShader(GetProgramId(), src->GetId());
    }

    bool Shader::GetLinkStatus()
    {
        int result;
        glGetProgramiv(GetProgramId(), GL_LINK_STATUS, &result);
        return result == GL_TRUE;
    }

    int Shader::GetProgramId()
    {
        return program_id;
    }

    bool Shader::IsShadersSupported()
    {
        bool hasVertex = true;    // IsExtensionSupported("GL_ARB_vertex_shader");
        bool hasFragment = true;  // IsExtensionSupported("GL_ARB_fragment_shader");
        return hasVertex && hasFragment;
    }

    void Shader::Bind(std::shared_ptr<Shader> shader)
    {
        glUseProgram(shader->GetProgramId());
        for (auto sb : shader->binds)
        {
            sb->Bind();
        }
    }

    void Shader::Unbind()
    {
        glUseProgram(0);
    }

    std::shared_ptr<Uniform> Shader::GetUniformFromName(const std::string& varname)
    {
        return std::make_shared<Uniform>(this, varname);
    }
}
