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

        angle ap(std::shared_ptr<Xml::Element> e, std::string p)
        {
            const auto f = dp(e, p);
            return angle::FromDegrees(f);
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
                r->sendToRenderer(target);
            }
        }
    }

    SimpleWorld::SimpleWorld(MediaLoader* loader, const std::string& file)
    {
        auto s = loader->FS()->open(file);
        auto level = Xml::Open(s, "level");
        addMeshes(loader, level->GetChild("world"), [this](auto x) { this->add(x); });
        addMeshes(loader, level->GetChild("camera"), [this](auto x) { this->addCamera(x); });
        addEntities(loader, level->GetChild("entity"));
    }

    void SimpleWorld::addEntities(MediaLoader* loader, std::shared_ptr<Xml::Element> level)
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
            addEntity(Entity::Create(loader, name, Xml::Open(gobstream, "object"), pos, rot));
        }
    }

    void SimpleWorld::addMeshes(MediaLoader* loader, std::shared_ptr<Xml::Element> level, RenderableAddTarget target)
    {
        for (std::shared_ptr<Xml::Element> entity : Xml::ElementsNamed(level, "mesh"))
        {
            // auto name = Xml::GetAttributeString(entity, "name");
            auto meshpath = Xml::GetAttributeString(entity, "file");
            // auto meshpath = t + ".mdf";
            auto mesh = std::make_shared<MeshInstance>(loader->fetch<Mesh>(meshpath));
            mesh->pos = GetPosition(entity->GetChild("position"));
            mesh->rot = GetRotation(entity->GetChild("rotation"));
            target(mesh);
        }
    }

    void SimpleWorld::add(std::shared_ptr<Renderable> r)
    {
        worldRenderables.push_back(r);
    }

    void SimpleWorld::remove(std::shared_ptr<Renderable> r)
    {
        Erase(worldRenderables, [r](std::shared_ptr<Renderable>& o) -> bool { return o == r; });
    }

    void SimpleWorld::worldSendTo(RenderList* list)
    {
        sendToList(list, worldRenderables);
    }

    void SimpleWorld::addCamera(std::shared_ptr<Renderable> r)
    {
        cameraRenderables.push_back(r);
    }

    void SimpleWorld::cameraSendTo(RenderList* list)
    {
        sendToList(list, cameraRenderables);
    }

    void SimpleWorld::addEntity(std::shared_ptr<Entity> ent)
    {
        entities.push_back(ent);
        for (auto r : ent->visual)
        {
            add(r);
        }
    }

    void SimpleWorld::clearScreen()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
