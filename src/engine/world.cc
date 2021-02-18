using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    public abstract class World
    {
        public abstract void add(Renderable r);
        public abstract void addCamera(Renderable r);
        public abstract void remove(Renderable r);
        public abstract void worldSendTo(RenderList list);
        public abstract void cameraSendTo(RenderList list);
        public abstract void addEntity(Entity ent);
        public static World Load(MediaLoader loader, string file)
        {
            return new SimpleWorld(loader, file);
        }
        public void render(int width, int height, Camera c)
        {
            Setup.view3d(width, height);
            using (PushedMatrix fm = new PushedMatrix())
            {
                c.sendRotation();
                RenderList list = new RenderList();
                cameraSendTo(list);
                list.render();
            }
            Gl.glClear(Gl.GL_DEPTH_BUFFER_BIT);
            using (PushedMatrix fm = new PushedMatrix())
            {
                c.sendRotationAndPosition();
                RenderList list = new RenderList();
                worldSendTo(list);
                list.render();
            }
        }
    }
}
