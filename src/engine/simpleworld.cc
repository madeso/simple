using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine
{
    struct SimpleWorld : World
    {
        std::vector<Renderable> worldRenderables = std::vector<Renderable>();

        std::vector<Renderable> cameraRenderables = std::vector<Renderable>();

        std::vector<Entity> entities = std::vector<Entity>();

        delegate void RenderableAddTarget(Renderable r);

        SimpleWorld(MediaLoader loader, std::string file)
        {
            using(var s = loader.FS.open(file))
            {
                XmlElement level = Xml.Open(Xml.FromStream(s), "level");
                addMeshes(loader, level["world"], add);
                addMeshes(loader, level["camera"], addCamera);
                addEntities(loader, level["entity"]);
            }
        }

        void addEntities(MediaLoader loader, XmlElement level)
        {
            for (XmlElement entity : Xml.ElementsNamed(level, "entity"))
            {
                std::string t = Xml.GetAttributeString(entity, "type");
                if (t[0] == '_')
                    continue;  // ignore for now
                std::string name = Xml.GetAttributeString(entity, "name");
                std::string meshpath = t + ".gob";
                vec3 pos = GetPosition(entity["position"]);
                quat rot = GetRotation(entity["rotation"]);

                using(var gobstream = loader.FS.open(meshpath))
                {
                    addEntity(Entity.Create(loader, name, Xml.Open(Xml.FromStream(gobstream), "object"), pos, rot));
                }
            }
        }

        void addMeshes(MediaLoader loader, XmlElement level, RenderableAddTarget target)
        {
            for (XmlElement entity : Xml.ElementsNamed(level, "entity"))
            {
                std::string t = Xml.GetAttributeString(entity, "type");
                std::string name = Xml.GetAttributeString(entity, "name");
                std::string meshpath = t + ".mdf";
                MeshInstance mesh = MeshInstance(loader.fetch<Mesh>(meshpath));
                mesh.pos = GetPosition(entity["position"]);
                mesh.rot = GetRotation(entity["rotation"]);
                target(mesh);
            }
        }

        static vec3 GetPosition(XmlElement e)
        {
            return vec3(dp(e, "x"), dp(e, "y"), dp(e, "z"));
        }

        static float dp(XmlElement e, std::string p)
        {
            std::string val = Xml.GetAttributeString(e, p);
            return float.Parse(val.Replace('.', ','));
        }

        static quat GetRotation(XmlElement e)
        {
            return quat(dp(e, "w"), vec3(dp(e, "x"), dp(e, "y"), dp(e, "z")));
        }

        override void add(Renderable r)
        {
            worldRenderables.Add(r);
        }

        override void remove(Renderable r)
        {
            worldRenderables.Remove(r);
        }

        override void worldSendTo(RenderList list)
        {
            sendToList(list, worldRenderables);
        }

        static void sendToList(RenderList target, std::vector<Renderable> container)
        {
            for (Renderable r : container)
            {
                r.sendToRenderer(target);
            }
        }

        override void addCamera(Renderable r)
        {
            cameraRenderables.Add(r);
        }

        override void cameraSendTo(RenderList list)
        {
            sendToList(list, cameraRenderables);
        }

        override void addEntity(Entity ent)
        {
            entities.Add(ent);
            for (Renderable r : ent.visual)
            {
                add(r);
            }
        }
    }
}
