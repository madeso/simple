#include "engine/entity.h"

#include <memory>
#include <string>
#include <vector>

#include "engine/medialoader.h"
#include "engine/mesh.h"
#include "engine/meshinstance.h"
#include "engine/quat.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    Entity::Entity(const std::string& n)
        : name(n)
    {
    }

    std::string Entity::ToString() const
    {
        return name;
    }

    std::shared_ptr<Entity> Entity::Create(MediaLoader* loader, const std::string& name, std::shared_ptr<Xml::Element> root, const vec3& pos, const quat& rot)
    {
        auto ent = std::make_shared<Entity>(name);

        auto visual = root->GetChild("visual");

        for (auto meshel : Xml::ElementsNamed(visual, "mesh"))
        {
            std::string meshpath = Xml::GetAttributeString(meshel, "file");
            auto mesh = std::make_shared<MeshInstance>(loader->GetOrLoad<Mesh>(meshpath));
            mesh->pos = pos;
            mesh->rot = rot;
            ent->AddRenderable(mesh);
        }

        return ent;
    }

    void Entity::AddRenderable(std::shared_ptr<Renderable> r)
    {
        renderables.emplace_back(r);
    }
}
