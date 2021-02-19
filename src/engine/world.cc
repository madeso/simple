using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    struct World
    {
        void add(Renderable r);
        void addCamera(Renderable r);
        void remove(Renderable r);
        void worldSendTo(RenderList list);
        void cameraSendTo(RenderList list);
        void addEntity(Entity ent);
        static World Load(MediaLoader loader, std::string file)
        {
            return SimpleWorld(loader, file);
        }
        void render(int width, int height, Camera c)
        {
            Setup.view3d(width, height);
            using(PushedMatrix fm = PushedMatrix())
            {
                c.sendRotation();
                RenderList list = RenderList();
                cameraSendTo(list);
                list.render();
            }
            Gl.glClear(Gl.GL_DEPTH_BUFFER_BIT);
            using(PushedMatrix fm = PushedMatrix())
            {
                c.sendRotationAndPosition();
                RenderList list = RenderList();
                worldSendTo(list);
                list.render();
            }
        }
    }
}
