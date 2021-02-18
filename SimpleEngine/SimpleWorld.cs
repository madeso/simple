using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace SimpleEngine
{
    public class SimpleWorld : World
    {
        private List<Renderable> worldRenderables = new List<Renderable>();
        private List<Renderable> cameraRenderables = new List<Renderable>();
        private List<Entity> entities = new List<Entity>();

        delegate void RenderableAddTarget(Renderable r);

        public SimpleWorld(MediaLoader loader, string file)
        {
            using (var s = loader.FS.open(file))
            {
                XmlElement level = Xml.Open(Xml.FromStream(s), "level");
                addMeshes(loader, level["world"], add);
                addMeshes(loader, level["camera"], addCamera);
                addEntities(loader, level["entity"]);
            }
        }

        private void addEntities(MediaLoader loader, XmlElement level)
        {
            foreach (XmlElement entity in Xml.ElementsNamed(level, "entity"))
            {
                string t = Xml.GetAttributeString(entity, "type");
                if (t[0] == '_') continue; // ignore for now
                string name = Xml.GetAttributeString(entity, "name");
                string meshpath = t + ".gob";
                vec3 pos = GetPosition(entity["position"]);
                quat rot = GetRotation(entity["rotation"]);

                using (var gobstream = loader.FS.open(meshpath))
                {
                    addEntity(Entity.Create(loader, name, Xml.Open(Xml.FromStream(gobstream), "object"), pos, rot));
                }
            }
        }

        private void addMeshes(MediaLoader loader, XmlElement level, RenderableAddTarget target)
        {
            foreach (XmlElement entity in Xml.ElementsNamed(level, "entity"))
            {
                string t = Xml.GetAttributeString(entity, "type");
                string name = Xml.GetAttributeString(entity, "name");
                string meshpath = t + ".mdf";
                MeshInstance mesh = new MeshInstance(loader.fetch<Mesh>(meshpath));
                mesh.pos = GetPosition(entity["position"]);
                mesh.rot = GetRotation(entity["rotation"]);
                target(mesh);
            }
        }

        private static vec3 GetPosition(XmlElement e)
        {
            return new vec3(dp(e, "x"), dp(e,"y"), dp(e, "z"));
        }

        private static float dp(XmlElement e, string p)
        {
            string val = Xml.GetAttributeString(e, p);
            return float.Parse(val.Replace('.', ','));
        }

        private static quat GetRotation(XmlElement e)
        {
            return new quat(dp(e, "w"), new vec3(dp(e, "x"), dp(e, "y"), dp(e, "z")));
        }

        public override void add(Renderable r)
        {
            worldRenderables.Add(r);
        }

        public override void remove(Renderable r)
        {
            worldRenderables.Remove(r);
        }

        public override void worldSendTo(RenderList list)
        {
            sendToList(list, worldRenderables);
        }

        private static void sendToList(RenderList target, List<Renderable> container)
        {
            foreach (Renderable r in container)
            {
                r.sendToRenderer(target);
            }
        }

        public override void addCamera(Renderable r)
        {
            cameraRenderables.Add(r);
        }

        public override void cameraSendTo(RenderList list)
        {
            sendToList(list, cameraRenderables);
        }

        public override void addEntity(Entity ent)
        {
            entities.Add(ent);
            foreach (Renderable r in ent.visual)
            {
                add(r);
            }
        }
    }
}
