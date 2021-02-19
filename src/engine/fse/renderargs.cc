using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine.fse
{
    struct RenderArgs
    {
        RenderArgs(World world, Camera cam, int width, int height)
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

        World World
        {
            get
            {
                return world;
            }
        }

        Camera Camera
        {
            get
            {
                return cam;
            }
        }

        int Width
        {
            get
            {
                return width;
            }
        }

        int Height
        {
            get
            {
                return height;
            }
        }

        void render()
        {
            World.render(Width, Height, Camera);
        }
    }
}
