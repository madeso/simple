using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using SimpleEngine;
using System.Xml;

namespace Convert
{
    class Convert
    {
        static void Main(string[] args)
        {
            string filepath = args[0];
            string dir = Path.GetDirectoryName(filepath);
            string filename = Path.GetFileName(filepath);
            FileSystem fs = new FileSystem();
            fs.addRoot(dir);
            fs.addRoot(Environment.CurrentDirectory);

            Actor actor = ActorFile.Load(fs, filename);

            string meshtarget = Path.Combine( dir, Path.ChangeExtension(filename,"mdf") );
            WriteMesh(actor.Mesh, meshtarget);

            foreach (KeyValuePair<string, Animation> animation in actor.Animations)
            {
                string filetarget = Path.ChangeExtension(Path.Combine(dir, animation.Key), "anm");
                WriteAnimation(filetarget, animation.Value);
            }
        }

        private static void WriteAnimation(string path, Animation an)
        {
            using (Stream s = File.OpenWrite(path))
            {
                AnimationFile.Write(an, s);
            }
        }

        private static void WriteMesh(MeshDef def, string path)
        {
            using (Stream s = File.OpenWrite(path))
            {
                MeshFile.Save(s, def);
            }
        }
    }
}
