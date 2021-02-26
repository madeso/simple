#include <iostream>
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
#include "engine/math.h"
#include "engine/meshfile.h"
#include "engine/strings.h"
#include "engine/xml.h"
#include "fmt/core.h"

namespace simple
{
    namespace actor_file
    {
        std::map<std::string, std::string> ParseOverrides(std::shared_ptr<xml::Element> root)
        {
            std::map<std::string, std::string> result;
            for (std::shared_ptr<xml::Element> e : xml::ElementsNamed(root, "overrride"))
            {
                const auto f = ToLower(xml::GetAttributeString(e, "from"));
                const auto t = ToLower(xml::GetAttributeString(e, "to"));
                result.emplace(f, t);
            }
            return result;
        }

        std::vector<AnimationInformation> ParseAnimationInfo(std::shared_ptr<xml::Element> root)
        {
            std::vector<AnimationInformation> animinfo;
            for (std::shared_ptr<xml::Element> e : xml::ElementsNamed(root, "animation"))
            {
                const auto ss = xml::GetTextOfSubElement(e, "start");
                const auto se = xml::GetTextOfSubElement(e, "end");
                const auto name = xml::GetTextOfSubElement(e, "name");
                int start = std::stoi(ss);
                int end = std::stoi(se);
                animinfo.push_back(AnimationInformation(start, end, name));
            }
            return animinfo;
        }

        std::set<std::string> ParseIgnoreBone(std::shared_ptr<xml::Element> root)
        {
            std::set<std::string> bonestoignore;
            for (std::shared_ptr<xml::Element> e : xml::ElementsNamed(root, "ignorebone"))
            {
                const auto ss = xml::GetAttributeString(e, "name");
                bonestoignore.emplace(ss);
            }
            return bonestoignore;
        }

        std::map<std::string, std::string> ParseSetTexture(std::shared_ptr<xml::Element> root)
        {
            std::map<std::string, std::string> texmap;
            for (std::shared_ptr<xml::Element> e : xml::ElementsNamed(root, "settex"))
            {
                const auto material = xml::GetAttributeString(e, "material");
                const auto texture = xml::GetAttributeString(e, "texture");
                texmap.emplace(material, texture);
            }
            return texmap;
        }

        std::shared_ptr<Actor> Load(FileSystem* fs, const std::string& p)
        {
            const auto full_path = fs->Open(p);
            std::shared_ptr<xml::Element> xactor = xml::Open(full_path, "actor");
            std::shared_ptr<xml::Element> xmesh = xactor->GetChild("mesh");

            const auto animinfo = ParseAnimationInfo(xmesh);
            const auto bonesToIgnore = ParseIgnoreBone(xmesh);
            auto texmap = ParseSetTexture(xmesh);
            const auto overrides = ParseOverrides(xmesh);

            fs->SetOverrides(overrides);

            const auto meshpath = xml::GetAttributeString(xmesh, "file");
            float scale = xml::GetAttribute<float>(xmesh, "scale", math::ParseFloat, 1.0f);

            std::shared_ptr<MeshDef> def;
            std::shared_ptr<Animation> animation;

            std::string ext = file_util::GetExtension(meshpath);
            if (ext == ".txt")
            {
                simple::load::milkshape_ascii::Load(fs, meshpath, &def, &animation, 1);
            }
            else if (ext == ".ms3d")
            {
                simple::load::milkshape_binary::Load(fs, meshpath, &def, &animation);
            }
            else if (animinfo.empty())
            {
                animation = nullptr;
                def = mesh_file::Load(fs, meshpath);
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
                        std::cout << "bug? reduce i by 1 since it was just removed?\n";
                    }
                }
            }

            for (auto mat : def->GetMaterials())
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

            def->ScaleMeshAndBones(scale);
            if (animation != nullptr)
                animation->Scale(scale);

            def->TranslateTexturePaths(overrides);

            def->MapBones();
            std::shared_ptr<Actor> actor = std::make_shared<Actor>(def);

            if (animation != nullptr)
            {
                for (AnimationInformation ai : animinfo)
                {
                    auto an = animation->GetSubAnimation(ai);
                    actor->AddAnimation(ai.name, std::make_shared<Animation>(an));
                }
            }

            fs->ClearOverrides(overrides);
            return actor;
        }
    }
}
