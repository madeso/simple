using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;
using System.Xml;

namespace SimpleEngine
{
    internal class ShaderSource
    {
        public ShaderSource(string name, string source, int type)
        {
            mShader = Gl.glCreateShader(type);
            var src = new string[] { source };
            Gl.glShaderSource(Shader, 1, ref src, new int[] { source.Length });
            Gl.glCompileShader(mShader);

            if (false == CompileStatus)
            {
                throw new Exception( "Shader " +name+ " failed to compile: " + getInfoLog() );
            }
        }

        private string getInfoLog()
        {
            int size;
            Gl.glGetShaderiv(Shader, Gl.GL_INFO_LOG_LENGTH, out size);

            string log;
            int length;
            Gl.glGetShaderInfoLog(Shader, size, out length, out log);

            return log;
        }

        private int mShader;

        private bool CompileStatus
        {
            get
            {
                int status;
                Gl.glGetShaderiv(Shader, Gl.GL_COMPILE_STATUS, out status);
                return status == Gl.GL_TRUE;
            }
        }

        public int Shader
        {
            get
            {
                return mShader;
            }
        }

        public const int Vertex = Gl.GL_VERTEX_SHADER;
        public const int Fragment = Gl.GL_FRAGMENT_SHADER;
    }

    internal class Uniform
    {
        int var;

        internal Uniform(Shader s, string name)
        {
            var = Gl.glGetUniformLocation(s.Program, ref name);
            if (var == -1) throw new Exception(name + " is not a recognized uniform");
        }

        internal void bindUniform(int location)
        {
            Gl.glUniform1i(var, location);
        }
        internal void bindUniform(float value)
        {
            Gl.glUniform1f(var, value);
        }

        internal void bindUniform(vec2 v)
        {
            Gl.glUniform2f(var, v.x, v.y);
        }
    }

    public class Shader
    {
        ShaderSource vertex;
        ShaderSource fragment;

        List<ShaderBind> binds = new List<ShaderBind>();

        public abstract class ShaderBind
        {
            public abstract void bind();
        }

        private class StaticUniformSamplerBind : ShaderBind
        {
            private readonly int location;
            Uniform var;

            public StaticUniformSamplerBind(XmlElement root, Shader shader)
            {
                string varname = Xml.GetAttributeString(root, "id");
                location = Xml.GetAttribute<int>(root, "location", int.Parse, -1);
                var = shader.getUniform(varname);
            }

            public override void bind()
            {
                var.bindUniform(location);
            }
        }

        public Shader(FileSystem sys, string path)
        {
            if (false == IsShadersSupported) throw new Exception("shaders not supported on your card, sorry");

            XmlElement shader = Xml.Open(Xml.FromStream(sys.open(path)), "shader");
            LoadFrom(path, shader);
        }

        public Shader(string path, XmlElement shader)
        {
            if (false == IsShadersSupported) throw new Exception("shaders not supported on your card, sorry");
            LoadFrom(path, shader);
        }

        private void LoadFrom(string path, XmlElement shader)
        {
            string vertexsource = Xml.GetTextOfSubElement(shader, "vertex");
            string fragmentsource = Xml.GetTextOfSubElement(shader, "fragment");

            mProgram = Gl.glCreateProgram();

            vertex = new ShaderSource(path + " (vert)", vertexsource, ShaderSource.Vertex);
            fragment = new ShaderSource(path + " (frag)", fragmentsource, ShaderSource.Fragment);

            attach(vertex);
            attach(fragment);
            Gl.glLinkProgram(Program);

            if (false == LinkStatus)
            {
                throw new Exception("Link error for " + path + ": " + getInfoLog());
            }

            foreach (XmlElement b in Xml.Elements(shader["bind"]))
            {
                binds.Add(new StaticUniformSamplerBind(b, this));
            }
        }

        private string getInfoLog()
        {
            int size;
            Gl.glGetProgramiv(Program, Gl.GL_INFO_LOG_LENGTH, out size);

            string log;
            int length;
            Gl.glGetProgramInfoLog(Program, size, out length, out log);

            return log;
        }

        private void attach(ShaderSource src)
        {
            Gl.glAttachShader(Program, src.Shader);
        }

        private bool LinkStatus
        {
            get
            {
                int result;
                Gl.glGetProgramiv(Program, Gl.GL_LINK_STATUS, out result);
                return result == Gl.GL_TRUE;
            }
        }

        internal int Program
        {
            get
            {
                return mProgram;
            }
        }

        private static bool IsShadersSupported
        {
            get
            {
                bool hasVertex = true;// Gl.IsExtensionSupported("GL_ARB_vertex_shader");
                bool hasFragment = true; // Gl.IsExtensionSupported("GL_ARB_fragment_shader");
                return hasVertex && hasFragment;
            }
        }

        private int mProgram;

        public static void Bind(Shader shader)
        {
            Gl.glUseProgram(shader.Program);
            foreach (ShaderBind sb in shader.binds)
            {
                sb.bind();
            }
        }

        public static void Unbind()
        {
            Gl.glUseProgram(0);
        }

        internal Uniform getUniform(string varname)
        {
            return new Uniform(this, varname);
        }
    }
}
