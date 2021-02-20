using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine
{
    struct Entity
    {
        std::vector<Renderable> visual = std::vector<Renderable>();
        std::string name;

        Entity(std::string name)
        {
            this.name = name;
        }

        std::string ToString() const
        {
            return name;
        }

        static Entity Create(MediaLoader loader, std::string name, System.Xml::std::shared_ptr<Xml::Element> root, vec3 pos, quat rot)
        {
            Entity ent = Entity(name);

            std::shared_ptr<Xml::Element> visual = root["visual"];

            for (std::shared_ptr<Xml::Element> meshel : Xml::ElementsNamed(visual, "mesh"))
            {
                std::string meshpath = Xml::GetAttributeString(meshel, "file");
                MeshInstance mesh = MeshInstance(loader.fetch<Mesh>(meshpath));
                mesh.pos = pos;
                mesh.rot = rot;
                ent.add(mesh);
            }

            return ent;
        }

        void add(Renderable r)
        {
            visual.Add(r);
        }
    }
}
