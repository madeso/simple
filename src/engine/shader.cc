using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;
using System.Xml;

namespace SimpleEngine
{
    struct ShaderSource
    {
        ShaderSource(std::string name, std::string source, int type)
        {
            mShader = glCreateShader(type);
            var src = std::string[]{source};
            glShaderSource(Shader, 1, ref src, int[]{source.Length});
            glCompileShader(mShader);

            if (false == CompileStatus)
            {
                throw std::runtime_error("Shader " + name + " failed to compile: " + getInfoLog());
            }
        }

        std::string getInfoLog()
        {
            int size;
            glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, out size);

            std::string log;
            int length;
            glGetShaderInfoLog(Shader, size, out length, out log);

            return log;
        }

        int mShader;

        bool CompileStatus
        {
            get
            {
                int status;
                glGetShaderiv(Shader, GL_COMPILE_STATUS, out status);
                return status == GL_TRUE;
            }
        }

        int Shader
        {
            get
            {
                return mShader;
            }
        }

        const int Vertex = GL_VERTEX_SHADER;
        const int Fragment = GL_FRAGMENT_SHADER;
    }

    struct Uniform
    {
        int var;

        Uniform(Shader s, std::string name)
        {
            var = glGetUniformLocation(s.Program, ref name);
            if (var == -1)
                throw std::runtime_error(name + " is not a recognized uniform");
        }

        void bindUniform(int location)
        {
            glUniform1i(var, location);
        }
        void bindUniform(float value)
        {
            glUniform1f(var, value);
        }

        void bindUniform(vec2 v)
        {
            glUniform2f(var, v.x, v.y);
        }
    }

    struct Shader
    {
        ShaderSource vertex;
        ShaderSource fragment;

        std::vector<ShaderBind> binds = std::vector<ShaderBind>();

        struct ShaderBind
        {
            void bind();
        }

        struct StaticUniformSamplerBind : ShaderBind
        {
            int location;
            Uniform var;

            StaticUniformSamplerBind(XmlElement root, Shader shader)
            {
                std::string varname = Xml.GetAttributeString(root, "id");
                location = Xml.GetAttribute<int>(root, "location", int.Parse, -1);
                var = shader.getUniform(varname);
            }

            override void bind()
            {
                var.bindUniform(location);
            }
        }

        Shader(FileSystem sys, std::string path)
        {
            if (false == IsShadersSupported)
                throw std::runtime_error("shaders not supported on your card, sorry");

            XmlElement shader = Xml.Open(Xml.FromStream(sys.open(path)), "shader");
            LoadFrom(path, shader);
        }

        Shader(std::string path, XmlElement shader)
        {
            if (false == IsShadersSupported)
                throw std::runtime_error("shaders not supported on your card, sorry");
            LoadFrom(path, shader);
        }

        void LoadFrom(std::string path, XmlElement shader)
        {
            std::string vertexsource = Xml.GetTextOfSubElement(shader, "vertex");
            std::string fragmentsource = Xml.GetTextOfSubElement(shader, "fragment");

            mProgram = glCreateProgram();

            vertex = ShaderSource(path + " (vert)", vertexsource, ShaderSource.Vertex);
            fragment = ShaderSource(path + " (frag)", fragmentsource, ShaderSource.Fragment);

            attach(vertex);
            attach(fragment);
            glLinkProgram(Program);

            if (false == LinkStatus)
            {
                throw std::runtime_error("Link error for " + path + ": " + getInfoLog());
            }

            for (XmlElement b : Xml.Elements(shader["bind"]))
            {
                binds.Add(StaticUniformSamplerBind(b, this));
            }
        }

        std::string getInfoLog()
        {
            int size;
            glGetProgramiv(Program, GL_INFO_LOG_LENGTH, out size);

            std::string log;
            int length;
            glGetProgramInfoLog(Program, size, out length, out log);

            return log;
        }

        void attach(ShaderSource src)
        {
            glAttachShader(Program, src.Shader);
        }

        bool LinkStatus
        {
            get
            {
                int result;
                glGetProgramiv(Program, GL_LINK_STATUS, out result);
                return result == GL_TRUE;
            }
        }

        int Program
        {
            get
            {
                return mProgram;
            }
        }

        static bool IsShadersSupported
        {
            get
            {
                bool hasVertex = true;    // IsExtensionSupported("GL_ARB_vertex_shader");
                bool hasFragment = true;  // IsExtensionSupported("GL_ARB_fragment_shader");
                return hasVertex && hasFragment;
            }
        }

        int mProgram;

        static void Bind(Shader shader)
        {
            glUseProgram(shader.Program);
            for (ShaderBind sb : shader.binds)
            {
                sb.bind();
            }
        }

        static void Unbind()
        {
            glUseProgram(0);
        }

        Uniform getUniform(std::string varname)
        {
            return Uniform(this, varname);
        }
    }
}
