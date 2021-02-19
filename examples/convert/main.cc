using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using SimpleEngine;
using System.Xml;

namespace Convert
{
    struct Convert
    {
        static void Main(std::string[] args)
        {
            std::string filepath = args[0];
            std::string dir = Path.GetDirectoryName(filepath);
            std::string filename = Path.GetFileName(filepath);
            FileSystem fs = FileSystem();
            fs.addRoot(dir);
            fs.addRoot(Environment.CurrentDirectory);

            Actor actor = ActorFile.Load(fs, filename);

            std::string meshtarget = Path.Combine(dir, Path.ChangeExtension(filename, "mdf"));
            WriteMesh(actor.Mesh, meshtarget);

            for (KeyValuePair<std::string, Animation> animation : actor.Animations)
            {
                std::string filetarget = Path.ChangeExtension(Path.Combine(dir, animation.Key), "anm");
                WriteAnimation(filetarget, animation.Value);
            }
        }

        static void WriteAnimation(std::string path, Animation an)
        {
            using(Stream s = File.OpenWrite(path))
            {
                AnimationFile.Write(an, s);
            }
        }

        static void WriteMesh(MeshDef def, std::string path)
        {
            using(Stream s = File.OpenWrite(path))
            {
                MeshFile.Save(s, def);
            }
        }
    }
}
