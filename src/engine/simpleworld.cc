#include "engine/simpleworld.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "engine/angle.h"
#include "engine/cpp.h"
#include "engine/filesystem.h"
#include "engine/math1.h"
#include "engine/medialoader.h"
#include "engine/mesh.h"
#include "engine/meshinstance.h"
#include "engine/opengl.h"
#include "engine/quat.h"
#include "engine/vec3.h"
#include "engine/world.h"

namespace SimpleEngine
{
    namespace
    {
        float dp(std::shared_ptr<Xml::Element> e, std::string p)
        {
            std::string val = Xml::GetAttributeString(e, p);
            return math1::ParseFloat(val);
        }

        Angle ap(std::shared_ptr<Xml::Element> e, std::string p)
        {
            const auto f = dp(e, p);
            return Angle::FromDegrees(f);
        }

        vec3 GetPosition(std::shared_ptr<Xml::Element> e)
        {
            return vec3(dp(e, "x"), dp(e, "y"), dp(e, "z"));
        }

        quat GetRotation(std::shared_ptr<Xml::Element> e)
        {
            return quat::FromYawPitchRoll(ap(e, "y"), ap(e, "p"), ap(e, "r"));
        }

        void sendToList(RenderList* target, const std::vector<std::shared_ptr<Renderable>>& container)
        {
            for (auto r : container)
            {
                r->SendToRenderer(target);
            }
        }
    }

    SimpleWorld::SimpleWorld(MediaLoader* loader, const std::string& file)
    {
        auto s = loader->FS()->open(file);
        auto level = Xml::Open(s, "level");
        AddMeshes(loader, level->GetChild("world"), [this](auto x) { this->Add(x); });
        AddMeshes(loader, level->GetChild("camera"), [this](auto x) { this->AddCameraRenderable(x); });
        AddEntities(loader, level->GetChild("entity"));
    }

    void SimpleWorld::AddEntities(MediaLoader* loader, std::shared_ptr<Xml::Element> level)
    {
        for (std::shared_ptr<Xml::Element> entity : Xml::ElementsNamed(level, "entity"))
        {
            std::string t = Xml::GetAttributeString(entity, "type");
            if (t[0] == '_')
                continue;  // ignore for now
            std::string name = Xml::GetAttributeString(entity, "name");
            std::string meshpath = t + ".gob";
            vec3 pos = GetPosition(entity->GetChild("position"));
            quat rot = GetRotation(entity->GetChild("rotation"));

            auto gobstream = loader->FS()->open(meshpath);
            AddEntity(Entity::Create(loader, name, Xml::Open(gobstream, "object"), pos, rot));
        }
    }

    void SimpleWorld::AddMeshes(MediaLoader* loader, std::shared_ptr<Xml::Element> level, RenderableAddTarget target)
    {
        for (std::shared_ptr<Xml::Element> entity : Xml::ElementsNamed(level, "mesh"))
        {
            // auto name = Xml::GetAttributeString(entity, "name");
            auto meshpath = Xml::GetAttributeString(entity, "file");
            // auto meshpath = t + ".mdf";
            auto mesh = std::make_shared<MeshInstance>(loader->GetOrLoad<Mesh>(meshpath));
            mesh->pos = GetPosition(entity->GetChild("position"));
            mesh->rot = GetRotation(entity->GetChild("rotation"));
            target(mesh);
        }
    }

    void SimpleWorld::Add(std::shared_ptr<Renderable> r)
    {
        world_renderables.push_back(r);
    }

    void SimpleWorld::Remove(std::shared_ptr<Renderable> r)
    {
        Erase(world_renderables, [r](std::shared_ptr<Renderable>& o) -> bool { return o == r; });
    }

    void SimpleWorld::SendWorldRenderablesToList(RenderList* list)
    {
        sendToList(list, world_renderables);
    }

    void SimpleWorld::AddCameraRenderable(std::shared_ptr<Renderable> r)
    {
        camera_renderables.push_back(r);
    }

    void SimpleWorld::SendCameraRenderablesToList(RenderList* list)
    {
        sendToList(list, camera_renderables);
    }

    void SimpleWorld::AddEntity(std::shared_ptr<Entity> ent)
    {
        entities.push_back(ent);
        for (auto r : ent->renderables)
        {
            Add(r);
        }
    }

    void SimpleWorld::ClearScreen()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
