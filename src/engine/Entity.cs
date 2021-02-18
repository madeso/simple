using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine
{
    public class Entity
    {
        public List<Renderable> visual = new List<Renderable>();
        public readonly string name;

        private Entity(string name)
        {
            this.name = name;
        }

        public override string ToString()
        {
            return name;
        }

        internal static Entity Create(MediaLoader loader, string name, System.Xml.XmlElement root, vec3 pos, quat rot)
        {
            Entity ent = new Entity(name);

            XmlElement visual = root["visual"];

            foreach(XmlElement meshel in Xml.ElementsNamed(visual, "mesh"))
            {
                string meshpath = Xml.GetAttributeString(meshel, "file");
                MeshInstance mesh = new MeshInstance(loader.fetch<Mesh>(meshpath));
                mesh.pos = pos;
                mesh.rot = rot;
                ent.add(mesh);
            }

            return ent;
        }

        private void add(Renderable r)
        {
            visual.Add(r);
        }
    }
}
