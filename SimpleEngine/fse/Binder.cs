using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    public class Binder
    {
        Map<Shader> shaders;

        public Binder(FileSystem sys)
        {
            shaders = new Map<Shader>(delegate(string name) { return new Shader(sys, name); });
        }

        internal Shader getShader(string shadername)
        {
            return shaders.get(shadername);
        }

        internal Shader getShaderOrNull(string shadername)
        {
            if (string.IsNullOrEmpty(shadername)) return null;
            return shaders.get(shadername);
        }

        List<BufferReference> references = new List<BufferReference>();

        internal void reference(BufferReference br)
        {
            if (references.Contains(br))
            {
                throw new Exception(br.Name + " already addd to " + this.ToString());
            }
            references.Add(br);
        }

        class FboCreator
        {
            Pool<Fbo> pool;
            Map<Fbo> map;

            public FboCreator(int width, int height)
            {
                pool = new Pool<Fbo>(delegate() { return new Fbo(width, height, false); });
                map = new Map<Fbo>(delegate(string n) { return pool.get(); });
            }

            public Fbo create(string name)
            {
                return map.get(name);
            }

            public void release(Fbo fbo)
            {
                pool.release(fbo);
            }
        }

        Dictionary<Size, FboCreator> creators = new Dictionary<Size, FboCreator>( new Size.Comperator() );

        //Pool<Fbo> cbs = new Pool<Fbo>(delegate() { return new Fbo(512, 512, false); });
        //Map<Fbo> buffers = new Map<Fbo>(delegate(string n) { return allocate(n); });
        private Fbo allocate(string name)
        {
            return getCreator(name).create(name);
        }

        private FboCreator getCreator(string name)
        {
            FboCreator c;
            Size size = sizeOf(name);
            if (false == creators.ContainsKey(size))
            {
                c = new FboCreator(size.Width, size.Height);
                creators.Add(size, c);
            }
            else
            {
                c = creators[size];
            }
            return c;
        }
        private void release(Fbo buff)
        {
            creators[new Size { Width=buff.Width, Height=buff.Height }].release(buff);
        }

        private Size sizeOf(string name)
        {
            if (false == associations.ContainsKey(name))
            {
                throw new Exception(name + " is missing a defined size");
            }
            else return associations[name];
        }

        internal void createBuffers()
        {
            StringCounter sc = new StringCounter();
            foreach (BufferReference b in references)
            {
                sc.add(b.Name);
            }

            StringCounter usages = new StringCounter();
            foreach (BufferReference b in references)
            {
                Fbo buff = allocate(b.Name);
                b.setBuffer(buff);
                usages.add(b.Name);
                if (usages.countsOf(b.Name) == sc.countsOf(b.Name))
                {
                    release(buff);
                }
            }
        }

        Dictionary<string, Size> associations = new Dictionary<string, Size>();
        internal void associate(string p, Size size)
        {
            associations.Add(p, size);
        }
    }
}
