using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace SimpleEngine
{
    public static class ActorFile
    {
        public static Actor Load(FileSystem fs, string p)
        {
            XmlElement xactor = Xml.Open(Xml.FromStream(fs.open(p)), "actor");
            XmlElement xmesh = xactor["mesh"];

            List<AnimationInformation> animinfo = ParseAnimationInfo(xmesh);
            List<string> bonesToIgnore = ParseIgnoreBone(xmesh);

            Dictionary<string, string> texmap = ParseSetTexture(xmesh);
            Dictionary<string, string> overrides = ParseOverrides(xmesh);

            fs.setOverrides(overrides);

            string meshpath = Xml.GetAttributeString(xmesh, "file");
            float scale =  Xml.GetAttribute<float>(xmesh, "scale", math1.ParseFloat, 1.0f);

            MeshDef def;
            Animation animation;

            string ext = Path.GetExtension(meshpath);
            if (ext == ".txt")
            {
                SimpleEngine.load.MilkshapeAscii.Load(fs, meshpath, out def, out animation, 1);
            }
            else if (ext == ".ms3d")
            {
                SimpleEngine.load.MilkshapeBinary.Load(fs, meshpath, out def, out animation);
            }
            else if( animinfo.Count==0 )
            {
                animation = null;
                def = MeshFile.Load(fs, meshpath);
            }
            else throw new Exception("Unhandled format " + ext + " for " + meshpath);

            foreach (string ignoreThisBone in bonesToIgnore)
            {
                for (int i = 0; i < def.bones.Count; ++i)
                {
                    if (ignoreThisBone == def.bones[i].name)
                    {
                        def.bones.RemoveAt(i);
                        animation.bones.RemoveAt(i);
                    }
                }
            }

            foreach (MeshDef.MaterialDef mat in def.Materials)
            {
                string matname = mat.name.ToLower();
                if (texmap.ContainsKey(matname))
                {
                    mat.texture = texmap[matname];
                    texmap.Remove(matname);
                }
            }

            if (texmap.Count != 0) throw new Exception("Some materials was not mapped");

            def.scale(scale);
            if( animation != null ) animation.scale(scale);

            def.translateFiles(overrides);

            Actor actor = new Actor(def.mapBones());

            if (animation != null)
            {
                foreach (AnimationInformation ai in animinfo)
                {
                    Animation an = animation.subanim(ai);
                    actor.add(ai.name, an);
                }
            }

            fs.clearOverrides(overrides);
            return actor;
        }

        private static Dictionary<string, string> ParseOverrides(XmlElement root)
        {
            Dictionary<string, string> result = new Dictionary<string, string>();
            foreach (XmlElement e in Xml.ElementsNamed(root, "overrride"))
            {
                string f = Xml.GetAttributeString(e, "from").ToLower();
                string t = Xml.GetAttributeString(e, "to").ToLower();
                result.Add(f, t);
            }
            return result;
        }

        private static List<AnimationInformation> ParseAnimationInfo(XmlElement root)
        {
            List<AnimationInformation> animinfo = new List<AnimationInformation>();
            foreach (XmlElement e in Xml.ElementsNamed(root, "animation"))
            {
                string ss = Xml.GetTextOfSubElement(e, "start");
                string se = Xml.GetTextOfSubElement(e, "end");
                string name = Xml.GetTextOfSubElement(e, "name");
                int start = int.Parse(ss);
                int end = int.Parse(se);
                animinfo.Add(new AnimationInformation(start, end, name));
            }
            return animinfo;
        }

        private static List<string> ParseIgnoreBone(XmlElement root)
        {
            List<string> bonestoignore = new List<string>();
            foreach (XmlElement e in Xml.ElementsNamed(root, "ignorebone"))
            {
                string ss = Xml.GetAttributeString(e, "name");
                bonestoignore.Add(ss);
            }
            return bonestoignore;
        }

        private static Dictionary<string, string> ParseSetTexture(XmlElement root)
        {
            Dictionary<string, string> texmap = new Dictionary<string, string>();
            foreach (XmlElement e in Xml.ElementsNamed(root, "settex"))
            {
                string material = Xml.GetAttributeString(e, "material");
                string texture = Xml.GetAttributeString(e, "texture");
                texmap.Add(material, texture);
            }
            return texmap;
        }
    }
}