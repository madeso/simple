using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    public class RenderArgs
    {
        public RenderArgs(World world, Camera cam, int width, int height)
        {
            this.world = world;
            this.cam = cam;
            this.width = width;
            this.height = height;
        }

        World world;
        Camera cam;
        int width;
        int height;

        public World World
        {
            get
            {
                return world;
            }
        }

        public Camera Camera
        {
            get
            {
                return cam;
            }
        }

        public int Width
        {
            get
            {
                return width;
            }
        }

        public int Height
        {
            get
            {
                return height;
            }
        }

        internal void render()
        {
            World.render(Width, Height, Camera);
        }
    }
}
