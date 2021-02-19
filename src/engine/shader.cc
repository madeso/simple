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
            mShader = Gl.glCreateShader(type);
            var src = std::string[]{source};
            Gl.glShaderSource(Shader, 1, ref src, int[]{source.Length});
            Gl.glCompileShader(mShader);

            if (false == CompileStatus)
            {
                throw Exception("Shader " + name + " failed to compile: " + getInfoLog());
            }
        }

        std::string getInfoLog()
        {
            int size;
            Gl.glGetShaderiv(Shader, Gl.GL_INFO_LOG_LENGTH, out size);

            std::string log;
            int length;
            Gl.glGetShaderInfoLog(Shader, size, out length, out log);

            return log;
        }

        int mShader;

        bool CompileStatus
        {
            get
            {
                int status;
                Gl.glGetShaderiv(Shader, Gl.GL_COMPILE_STATUS, out status);
                return status == Gl.GL_TRUE;
            }
        }

        int Shader
        {
            get
            {
                return mShader;
            }
        }

        const int Vertex = Gl.GL_VERTEX_SHADER;
        const int Fragment = Gl.GL_FRAGMENT_SHADER;
    }

    struct Uniform
    {
        int var;

        Uniform(Shader s, std::string name)
        {
            var = Gl.glGetUniformLocation(s.Program, ref name);
            if (var == -1)
                throw Exception(name + " is not a recognized uniform");
        }

        void bindUniform(int location)
        {
            Gl.glUniform1i(var, location);
        }
        void bindUniform(float value)
        {
            Gl.glUniform1f(var, value);
        }

        void bindUniform(vec2 v)
        {
            Gl.glUniform2f(var, v.x, v.y);
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
                throw Exception("shaders not supported on your card, sorry");

            XmlElement shader = Xml.Open(Xml.FromStream(sys.open(path)), "shader");
            LoadFrom(path, shader);
        }

        Shader(std::string path, XmlElement shader)
        {
            if (false == IsShadersSupported)
                throw Exception("shaders not supported on your card, sorry");
            LoadFrom(path, shader);
        }

        void LoadFrom(std::string path, XmlElement shader)
        {
            std::string vertexsource = Xml.GetTextOfSubElement(shader, "vertex");
            std::string fragmentsource = Xml.GetTextOfSubElement(shader, "fragment");

            mProgram = Gl.glCreateProgram();

            vertex = ShaderSource(path + " (vert)", vertexsource, ShaderSource.Vertex);
            fragment = ShaderSource(path + " (frag)", fragmentsource, ShaderSource.Fragment);

            attach(vertex);
            attach(fragment);
            Gl.glLinkProgram(Program);

            if (false == LinkStatus)
            {
                throw Exception("Link error for " + path + ": " + getInfoLog());
            }

            for (XmlElement b : Xml.Elements(shader["bind"]))
            {
                binds.Add(StaticUniformSamplerBind(b, this));
            }
        }

        std::string getInfoLog()
        {
            int size;
            Gl.glGetProgramiv(Program, Gl.GL_INFO_LOG_LENGTH, out size);

            std::string log;
            int length;
            Gl.glGetProgramInfoLog(Program, size, out length, out log);

            return log;
        }

        void attach(ShaderSource src)
        {
            Gl.glAttachShader(Program, src.Shader);
        }

        bool LinkStatus
        {
            get
            {
                int result;
                Gl.glGetProgramiv(Program, Gl.GL_LINK_STATUS, out result);
                return result == Gl.GL_TRUE;
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
                bool hasVertex = true;    // Gl.IsExtensionSupported("GL_ARB_vertex_shader");
                bool hasFragment = true;  // Gl.IsExtensionSupported("GL_ARB_fragment_shader");
                return hasVertex && hasFragment;
            }
        }

        int mProgram;

        static void Bind(Shader shader)
        {
            Gl.glUseProgram(shader.Program);
            for (ShaderBind sb : shader.binds)
            {
                sb.bind();
            }
        }

        static void Unbind()
        {
            Gl.glUseProgram(0);
        }

        Uniform getUniform(std::string varname)
        {
            return Uniform(this, varname);
        }
    }
}
