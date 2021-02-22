#include <map>
#include <set>
#include <stdexcept>
#include <vector>

#include "engine/actor.h"
#include "engine/animation.h"
#include "engine/cpp.h"
#include "engine/filesystem.h"
#include "engine/fileutil.h"
#include "engine/load/milkshape.ascii.h"
#include "engine/load/milkshape.binary.h"
#include "engine/math1.h"
#include "engine/meshfile.h"
#include "engine/strings.h"
#include "engine/xml.h"
#include "fmt/core.h"

namespace SimpleEngine
{
    namespace ActorFile
    {
        std::map<std::string, std::string> ParseOverrides(std::shared_ptr<Xml::Element> root)
        {
            std::map<std::string, std::string> result;
            for (std::shared_ptr<Xml::Element> e : Xml::ElementsNamed(root, "overrride"))
            {
                const auto f = ToLower(Xml::GetAttributeString(e, "from"));
                const auto t = ToLower(Xml::GetAttributeString(e, "to"));
                result.emplace(f, t);
            }
            return result;
        }

        std::vector<AnimationInformation> ParseAnimationInfo(std::shared_ptr<Xml::Element> root)
        {
            std::vector<AnimationInformation> animinfo;
            for (std::shared_ptr<Xml::Element> e : Xml::ElementsNamed(root, "animation"))
            {
                const auto ss = Xml::GetTextOfSubElement(e, "start");
                const auto se = Xml::GetTextOfSubElement(e, "end");
                const auto name = Xml::GetTextOfSubElement(e, "name");
                int start = std::stoi(ss);
                int end = std::stoi(se);
                animinfo.push_back(AnimationInformation(start, end, name));
            }
            return animinfo;
        }

        std::set<std::string> ParseIgnoreBone(std::shared_ptr<Xml::Element> root)
        {
            std::set<std::string> bonestoignore;
            for (std::shared_ptr<Xml::Element> e : Xml::ElementsNamed(root, "ignorebone"))
            {
                const auto ss = Xml::GetAttributeString(e, "name");
                bonestoignore.emplace(ss);
            }
            return bonestoignore;
        }

        std::map<std::string, std::string> ParseSetTexture(std::shared_ptr<Xml::Element> root)
        {
            std::map<std::string, std::string> texmap;
            for (std::shared_ptr<Xml::Element> e : Xml::ElementsNamed(root, "settex"))
            {
                const auto material = Xml::GetAttributeString(e, "material");
                const auto texture = Xml::GetAttributeString(e, "texture");
                texmap.emplace(material, texture);
            }
            return texmap;
        }

        std::shared_ptr<Actor> Load(FileSystem* fs, const std::string& p)
        {
            const auto full_path = fs->open(p);
            std::shared_ptr<Xml::Element> xactor = Xml::Open(full_path, "actor");
            std::shared_ptr<Xml::Element> xmesh = xactor->GetChild("mesh");

            const auto animinfo = ParseAnimationInfo(xmesh);
            const auto bonesToIgnore = ParseIgnoreBone(xmesh);
            auto texmap = ParseSetTexture(xmesh);
            const auto overrides = ParseOverrides(xmesh);

            fs->setOverrides(overrides);

            const auto meshpath = Xml::GetAttributeString(xmesh, "file");
            float scale = Xml::GetAttribute<float>(xmesh, "scale", math1::ParseFloat, 1.0f);

            std::shared_ptr<MeshDef> def;
            std::shared_ptr<Animation> animation;

            std::string ext = FileUtil::GetExtension(meshpath);
            if (ext == ".txt")
            {
                SimpleEngine::load::MilkshapeAscii::Load(fs, meshpath, &def, &animation, 1);
            }
            else if (ext == ".ms3d")
            {
                SimpleEngine::load::MilkshapeBinary::Load(fs, meshpath, &def, &animation);
            }
            else if (animinfo.empty())
            {
                animation = nullptr;
                def = MeshFile::Load(fs, meshpath);
            }
            else
            {
                throw std::runtime_error(fmt::format("Unhandled format {} for {}", ext, meshpath));
            }

            for (std::string ignoreThisBone : bonesToIgnore)
            {
                for (int i = 0; i < def->bones.size(); ++i)
                {
                    if (ignoreThisBone == def->bones[i]->name)
                    {
                        def->bones.erase(def->bones.begin() + i);
                        animation->bones.erase(animation->bones.begin() + i);
                        // bug? reduce i by 1 since it was just removed?
                    }
                }
            }

            for (auto mat : def->Materials())
            {
                std::string matname = ToLower(mat->name);
                if (auto found = texmap.find(matname); found != texmap.end())
                {
                    mat->texture = found->second;
                    texmap.erase(found);
                }
            }

            if (texmap.empty() == false)
                throw std::runtime_error("Some materials was not mapped");

            def->scale(scale);
            if (animation != nullptr)
                animation->scale(scale);

            def->translateFiles(overrides);

            def->mapBones();
            std::shared_ptr<Actor> actor = std::make_shared<Actor>(def);

            if (animation != nullptr)
            {
                for (AnimationInformation ai : animinfo)
                {
                    auto an = animation->subanim(ai);
                    actor->add(ai.name, std::make_shared<Animation>(an));
                }
            }

            fs->clearOverrides(overrides);
            return actor;
        }
    }
}
