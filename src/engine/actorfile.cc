using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace SimpleEngine
{
    namespace ActorFile
    {
        static Actor Load(FileSystem fs, std::string p)
        {
            XmlElement xactor = Xml.Open(Xml.FromStream(fs.open(p)), "actor");
            XmlElement xmesh = xactor["mesh"];

            std::vector<AnimationInformation> animinfo = ParseAnimationInfo(xmesh);
            std::vector<std::string> bonesToIgnore = ParseIgnoreBone(xmesh);

            std::map<std::string, std::string> texmap = ParseSetTexture(xmesh);
            std::map<std::string, std::string> overrides = ParseOverrides(xmesh);

            fs.setOverrides(overrides);

            std::string meshpath = Xml.GetAttributeString(xmesh, "file");
            float scale = Xml.GetAttribute<float>(xmesh, "scale", math1.ParseFloat, 1.0f);

            MeshDef def;
            Animation animation;

            std::string ext = Path.GetExtension(meshpath);
            if (ext == ".txt")
            {
                SimpleEngine.load.MilkshapeAscii.Load(fs, meshpath, out def, out animation, 1);
            }
            else if (ext == ".ms3d")
            {
                SimpleEngine.load.MilkshapeBinary.Load(fs, meshpath, out def, out animation);
            }
            else if (animinfo.Count == 0)
            {
                animation = nullptr;
                def = MeshFile.Load(fs, meshpath);
            }
            else
                throw Exception("Unhandled format " + ext + " for " + meshpath);

            for (std::string ignoreThisBone : bonesToIgnore)
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

            for (MeshDef.MaterialDef mat : def.Materials)
            {
                std::string matname = mat.name.ToLower();
                if (texmap.ContainsKey(matname))
                {
                    mat.texture = texmap[matname];
                    texmap.Remove(matname);
                }
            }

            if (texmap.Count != 0)
                throw Exception("Some materials was not mapped");

            def.scale(scale);
            if (animation != nullptr)
                animation.scale(scale);

            def.translateFiles(overrides);

            Actor actor = Actor(def.mapBones());

            if (animation != nullptr)
            {
                for (AnimationInformation ai : animinfo)
                {
                    Animation an = animation.subanim(ai);
                    actor.add(ai.name, an);
                }
            }

            fs.clearOverrides(overrides);
            return actor;
        }

        static std::map<std::string, std::string> ParseOverrides(XmlElement root)
        {
            std::map<std::string, std::string> result = std::map<std::string, std::string>();
            for (XmlElement e : Xml.ElementsNamed(root, "overrride"))
            {
                std::string f = Xml.GetAttributeString(e, "from").ToLower();
                std::string t = Xml.GetAttributeString(e, "to").ToLower();
                result.Add(f, t);
            }
            return result;
        }

        static std::vector<AnimationInformation> ParseAnimationInfo(XmlElement root)
        {
            std::vector<AnimationInformation> animinfo = std::vector<AnimationInformation>();
            for (XmlElement e : Xml.ElementsNamed(root, "animation"))
            {
                std::string ss = Xml.GetTextOfSubElement(e, "start");
                std::string se = Xml.GetTextOfSubElement(e, "end");
                std::string name = Xml.GetTextOfSubElement(e, "name");
                int start = int.Parse(ss);
                int end = int.Parse(se);
                animinfo.Add(AnimationInformation(start, end, name));
            }
            return animinfo;
        }

        static std::vector<std::string> ParseIgnoreBone(XmlElement root)
        {
            std::vector<std::string> bonestoignore = std::vector<std::string>();
            for (XmlElement e : Xml.ElementsNamed(root, "ignorebone"))
            {
                std::string ss = Xml.GetAttributeString(e, "name");
                bonestoignore.Add(ss);
            }
            return bonestoignore;
        }

        static std::map<std::string, std::string> ParseSetTexture(XmlElement root)
        {
            std::map<std::string, std::string> texmap = std::map<std::string, std::string>();
            for (XmlElement e : Xml.ElementsNamed(root, "settex"))
            {
                std::string material = Xml.GetAttributeString(e, "material");
                std::string texture = Xml.GetAttributeString(e, "texture");
                texmap.Add(material, texture);
            }
            return texmap;
        }
    }
}
