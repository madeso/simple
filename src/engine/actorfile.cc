#include <stdexcept>

#include "engine/actor.h"
#include "engine/animation.h"
#include "engine/cpp.h"
#include "engine/filesystem.h"
#include "engine/math1.h"
#include "engine/xml.h"

namespace SimpleEngine
{
    namespace ActorFile
    {
        Actor Load(FileSystem* fs, const std::string& p)
        {
            const auto full_path = fs->open(p);
            std::shared_ptr<Xml::Element> xactor = Xml::Open(full_path, "actor");
            std::shared_ptr<Xml::Element> xmesh = xactor->GetChild("mesh");

            std::vector<AnimationInformation> animinfo = ParseAnimationInfo(xmesh);
            std::vector<std::string> bonesToIgnore = ParseIgnoreBone(xmesh);

            std::map<std::string, std::string> texmap = ParseSetTexture(xmesh);
            std::map<std::string, std::string> overrides = ParseOverrides(xmesh);

            fs->setOverrides(overrides);

            std::string meshpath = Xml::GetAttributeString(xmesh, "file");
            float scale = Xml::GetAttribute<float>(xmesh, "scale", math1::ParseFloat, 1.0f);

            MeshDef def;
            std::shared_ptr<Animation> animation;

            std::string ext = Path.GetExtension(meshpath);
            if (ext == ".txt")
            {
                SimpleEngine::load::MilkshapeAscii.Load(fs, meshpath, &def, &animation, 1);
            }
            else if (ext == ".ms3d")
            {
                SimpleEngine::load::MilkshapeBinary.Load(fs, meshpath, &def, &animation);
            }
            else if (animinfo.empty())
            {
                animation = nullptr;
                def = MeshFile::Load(fs, meshpath);
            }
            else
                throw std::runtime_error("Unhandled format " + ext + " for " + meshpath);

            for (std::string ignoreThisBone : bonesToIgnore)
            {
                for (int i = 0; i < def.bones.size(); ++i)
                {
                    if (ignoreThisBone == def.bones[i]->name)
                    {
                        def.bones.erase(def.bones.begin() + i);
                        animation->bones.erase(animation->bones.begin() + i);
                    }
                }
            }

            for (MaterialDef mat : def.Materials)
            {
                std::string matname = mat.name.ToLower();
                if (texmap.ContainsKey(matname))
                {
                    mat.texture = texmap[matname];
                    texmap.Remove(matname);
                }
            }

            if (texmap.empty() == false)
                throw std::runtime_error("Some materials was not mapped");

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

        std::map<std::string, std::string> ParseOverrides(std::shared_ptr<Xml::Element> root)
        {
            std::map<std::string, std::string> result = std::map<std::string, std::string>();
            for (std::shared_ptr<Xml::Element> e : Xml::ElementsNamed(root, "overrride"))
            {
                std::string f = Lower(Xml::GetAttributeString(e, "from"));
                std::string t = Lower(Xml::GetAttributeString(e, "to"));
                result.emplace(f, t);
            }
            return result;
        }

        std::vector<AnimationInformation> ParseAnimationInfo(std::shared_ptr<Xml::Element> root)
        {
            std::vector<AnimationInformation> animinfo = std::vector<AnimationInformation>();
            for (std::shared_ptr<Xml::Element> e : Xml::ElementsNamed(root, "animation"))
            {
                std::string ss = Xml::GetTextOfSubElement(e, "start");
                std::string se = Xml::GetTextOfSubElement(e, "end");
                std::string name = Xml::GetTextOfSubElement(e, "name");
                int start = std::stoi(ss);
                int end = std::stoi(se);
                animinfo.push_back(AnimationInformation(start, end, name));
            }
            return animinfo;
        }

        std::vector<std::string> ParseIgnoreBone(std::shared_ptr<Xml::Element> root)
        {
            std::vector<std::string> bonestoignore = std::vector<std::string>();
            for (std::shared_ptr<Xml::Element> e : Xml::ElementsNamed(root, "ignorebone"))
            {
                std::string ss = Xml::GetAttributeString(e, "name");
                bonestoignore.push_back(ss);
            }
            return bonestoignore;
        }

        std::map<std::string, std::string> ParseSetTexture(std::shared_ptr<Xml::Element> root)
        {
            std::map<std::string, std::string> texmap = std::map<std::string, std::string>();
            for (std::shared_ptr<Xml::Element> e : Xml::ElementsNamed(root, "settex"))
            {
                std::string material = Xml::GetAttributeString(e, "material");
                std::string texture = Xml::GetAttributeString(e, "texture");
                texmap.emplace(material, texture);
            }
            return texmap;
        }
    }
}
