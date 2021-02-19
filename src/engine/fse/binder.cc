using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    struct Binder
    {
        Map<Shader> shaders;

        Binder(FileSystem sys)
        {
            shaders = Map<Shader>(delegate(std::string name) { return Shader(sys, name); });
        }

        Shader getShader(std::string shadername)
        {
            return shaders.get(shadername);
        }

        Shader getShaderOrNull(std::string shadername)
        {
            if (std::string.IsNullOrEmpty(shadername))
                return nullptr;
            return shaders.get(shadername);
        }

        std::vector<BufferReference> references = std::vector<BufferReference>();

        void reference(BufferReference br)
        {
            if (references.Contains(br))
            {
                throw Exception(br.Name + " already addd to " + this.ToString());
            }
            references.Add(br);
        }

        struct FboCreator
        {
            Pool<Fbo> pool;
            Map<Fbo> map;

            FboCreator(int width, int height)
            {
                pool = Pool<Fbo>(delegate() { return Fbo(width, height, false); });
                map = Map<Fbo>(delegate(std::string n) { return pool.get(); });
            }

            Fbo create(std::string name)
            {
                return map.get(name);
            }

            void release(Fbo fbo)
            {
                pool.release(fbo);
            }
        }

        std::map<Size, FboCreator>
            creators = std::map<Size, FboCreator>(Size.Comperator());

        //Pool<Fbo> cbs = Pool<Fbo>(delegate() { return Fbo(512, 512, false); });
        //Map<Fbo> buffers = Map<Fbo>(delegate(std::string n) { return allocate(n); });

        Fbo allocate(std::string name)
        {
            return getCreator(name).create(name);
        }

        FboCreator getCreator(std::string name)
        {
            FboCreator c;
            Size size = sizeOf(name);
            if (false == creators.ContainsKey(size))
            {
                c = FboCreator(size.Width, size.Height);
                creators.Add(size, c);
            }
            else
            {
                c = creators[size];
            }
            return c;
        }

        void release(Fbo buff)
        {
            creators[Size{Width = buff.Width, Height = buff.Height}].release(buff);
        }

        Size sizeOf(std::string name)
        {
            if (false == associations.ContainsKey(name))
            {
                throw Exception(name + " is missing a defined size");
            }
            else
                return associations[name];
        }

        void createBuffers()
        {
            StringCounter sc = StringCounter();
            for (BufferReference b : references)
            {
                sc.add(b.Name);
            }

            StringCounter usages = StringCounter();
            for (BufferReference b : references)
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

        std::map<std::string, Size> associations = std::map<std::string, Size>();
        void associate(std::string p, Size size)
        {
            associations.Add(p, size);
        }
    }
}
