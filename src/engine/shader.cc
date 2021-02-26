#include "engine/shader.h"

#include <cstdint>
#include <stdexcept>
#include <string>

#include "engine/filesystem.h"
#include "engine/opengl.h"
#include "engine/strings.h"
#include "engine/vec2.h"
#include "fmt/core.h"

namespace simple
{
    Shader::Shader(const std::string& name, const std::string& source, ShaderType type)
    {
        id = glCreateShader(type);
        int length = source.length();
        std::vector<char> string_source;
        string_source.resize(source.length()+1, 0);
        strcpy(string_source.data(), source.c_str());
        char* cstyle_source = string_source.data();
        glShaderSource(id, 1, &cstyle_source, &length);
        glCompileShader(id);

        if (false == GetCompileStatus())
        {
            throw std::runtime_error(fmt::format("Shader {} failed to compile: {}", name, GetInfoLog()));
        }
    }

    std::string Shader::GetInfoLog()
    {
        int size;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);

        std::vector<char> log;
        log.resize(size + 1, 0);
        int length;
        glGetShaderInfoLog(id, size, &length, &log[0]);

        return &log[0];
    }

    bool Shader::GetCompileStatus()
    {
        int status;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);
        return status == GL_TRUE;
    }

    Uniform::Uniform(ShaderProgram* program, const std::string& name)
    {
        id = glGetUniformLocation(program->id, name.c_str());
        if (id == -1)
            throw std::runtime_error(fmt::format("{} is not a recognized uniform", name));
    }

    void Uniform::SetTexture(int location)
    {
        glUniform1i(id, location);
    }

    void Uniform::Set(float value)
    {
        glUniform1f(id, value);
    }

    void Uniform::Set(const vec2& value)
    {
        glUniform2f(id, value.x, value.y);
    }

    StaticUniformSamplerBind::StaticUniformSamplerBind(std::shared_ptr<xml::Element> root, ShaderProgram* program)
    {
        std::string var_name = xml::GetAttributeString(root, "id");
        location = xml::GetAttribute<int>(
            root, "location", [](const std::string& s) -> int { return std::stoi(s); }, -1);
        uniform = program->GetUniformFromName(var_name);
    }

    void StaticUniformSamplerBind::Bind()
    {
        uniform->SetTexture(location);
    }

    ShaderProgram::ShaderProgram(FileSystem* fs, const std::string& path)
    {
        std::shared_ptr<xml::Element> shader_root = xml::Open(fs->Open(path), "shader");
        LoadFrom(path, shader_root);
    }

    ShaderProgram::ShaderProgram(const std::string& path, std::shared_ptr<xml::Element> shader_root)
    {
        LoadFrom(path, shader_root);
    }

    void ShaderProgram::LoadFrom(const std::string& path, std::shared_ptr<xml::Element> shader_root)
    {
        std::string vertex_source = Trim(xml::GetTextOfSubElement(shader_root, "vertex"));
        std::string fragment_source = Trim(xml::GetTextOfSubElement(shader_root, "fragment"));

        id = glCreateProgram();

        vertex_shader = std::make_shared<Shader>(path + " (vert)", vertex_source, Shader::Vertex);
        fragment_shader = std::make_shared<Shader>(path + " (frag)", fragment_source, Shader::Fragment);

        Attach(vertex_shader);
        Attach(fragment_shader);
        glLinkProgram(id);

        if (false == GetLinkStatus())
        {
            throw std::runtime_error("Link error for " + path + ": " + GetInfoLog());
        }

        for (auto bind_root : xml::Elements(shader_root->GetChild("bind")))
        {
            binds.emplace_back(std::make_shared<StaticUniformSamplerBind>(bind_root, this));
        }
    }

    std::string ShaderProgram::GetInfoLog() const
    {
        int length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

        std::vector<char> log;
        log.resize(length + 1, 0);
        int junk_length;
        glGetProgramInfoLog(id, length, &junk_length, &log[0]);

        return &log[0];
    }

    void ShaderProgram::Attach(std::shared_ptr<Shader> shader)
    {
        glAttachShader(id, shader->id);
    }

    bool ShaderProgram::GetLinkStatus()
    {
        int result;
        glGetProgramiv(id, GL_LINK_STATUS, &result);
        return result == GL_TRUE;
    }

    void ShaderProgram::Bind(std::shared_ptr<ShaderProgram> program)
    {
        glUseProgram(program->id);
        for (auto sb : program->binds)
        {
            sb->Bind();
        }
    }

    void ShaderProgram::Unbind()
    {
        glUseProgram(0);
    }

    std::shared_ptr<Uniform> ShaderProgram::GetUniformFromName(const std::string& name)
    {
        return std::make_shared<Uniform>(this, name);
    }
}
