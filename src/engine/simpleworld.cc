#include "engine/simpleworld.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "engine/angle.h"
#include "engine/cpp.h"
#include "engine/filesystem.h"
#include "engine/math.h"
#include "engine/medialoader.h"
#include "engine/mesh.h"
#include "engine/meshinstance.h"
#include "engine/opengl.h"
#include "engine/quat.h"
#include "engine/vec3.h"
#include "engine/world.h"

namespace simple
{
    namespace
    {
        float ReadFloat(std::shared_ptr<xml::Element> element, const std::string& attribute_name)
        {
            const std::string value = xml::GetAttributeString(element, attribute_name);
            return math::ParseFloat(value);
        }

        Angle ReadAngle(std::shared_ptr<xml::Element> element, const std::string& attribute_name)
        {
            const auto f = ReadFloat(element, attribute_name);
            return Angle::FromDegrees(f);
        }

        vec3 GetPosition(std::shared_ptr<xml::Element> e)
        {
            return vec3(ReadFloat(e, "x"), ReadFloat(e, "y"), ReadFloat(e, "z"));
        }

        quat GetRotation(std::shared_ptr<xml::Element> e)
        {
            return quat::FromYawPitchRoll(ReadAngle(e, "y"), ReadAngle(e, "p"), ReadAngle(e, "r"));
        }

        void SendToList(RenderList* target, const std::vector<std::shared_ptr<Renderable>>& container)
        {
            for (const auto& renderable : container)
            {
                renderable->SendToRenderer(target);
            }
        }
    }

    SimpleWorld::SimpleWorld(MediaLoader* loader, const std::string& file)
    {
        auto absolute_path = loader->FS()->Open(file);
        auto level = xml::Open(absolute_path, "level");
        AddMeshes(loader, level->GetChild("world"), [this](auto x) { this->Add(x); });
        AddMeshes(loader, level->GetChild("camera"), [this](auto x) { this->AddCameraRenderable(x); });
        AddEntities(loader, level->GetChild("entity"));
    }

    void SimpleWorld::AddEntities(MediaLoader* loader, std::shared_ptr<xml::Element> level)
    {
        for (const std::shared_ptr<xml::Element>& entity : xml::ElementsNamed(level, "entity"))
        {
            auto type = xml::GetAttributeString(entity, "type");
            if (type[0] == '_')
                continue;  // ignore for now
            std::string name = xml::GetAttributeString(entity, "name");
            std::string meshpath = type + ".gob";
            vec3 pos = GetPosition(entity->GetChild("position"));
            quat rot = GetRotation(entity->GetChild("rotation"));

            auto gobstream = loader->FS()->Open(meshpath);
            AddEntity(Entity::Create(loader, name, xml::Open(gobstream, "object"), pos, rot));
        }
    }

    void SimpleWorld::AddMeshes(MediaLoader* loader, std::shared_ptr<xml::Element> level, const RenderableAddTarget& target)
    {
        for (std::shared_ptr<xml::Element> entity : xml::ElementsNamed(level, "mesh"))
        {
            // auto name = Xml::GetAttributeString(entity, "name");
            auto meshpath = xml::GetAttributeString(entity, "file");
            // auto meshpath = t + ".mdf";
            auto mesh = std::make_shared<MeshInstance>(loader->GetOrLoad<Mesh>(meshpath));
            mesh->position = GetPosition(entity->GetChild("position"));
            mesh->rotation = GetRotation(entity->GetChild("rotation"));
            target(mesh);
        }
    }

    void SimpleWorld::Add(std::shared_ptr<Renderable> r)
    {
        world_renderables.push_back(r);
    }

    void SimpleWorld::Remove(std::shared_ptr<Renderable> renderable)
    {
        Erase(world_renderables, [renderable](std::shared_ptr<Renderable>& other) -> bool { return other == renderable; });
    }

    void SimpleWorld::SendWorldRenderablesToList(RenderList* list)
    {
        SendToList(list, world_renderables);
    }

    void SimpleWorld::AddCameraRenderable(std::shared_ptr<Renderable> renderable)
    {
        camera_renderables.push_back(renderable);
    }

    void SimpleWorld::SendCameraRenderablesToList(RenderList* list)
    {
        SendToList(list, camera_renderables);
    }

    void SimpleWorld::AddEntity(std::shared_ptr<Entity> entity)
    {
        entities.push_back(entity);
        for (auto r : entity->renderables)
        {
            Add(r);
        }
    }

    void SimpleWorld::ClearScreen()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
