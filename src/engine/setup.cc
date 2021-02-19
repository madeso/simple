using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Tao.OpenGl;

namespace SimpleEngine
{
    namespace Setup
    {
        static void basicOpenGL()
        {
            glShadeModel(GL_SMOOTH);
            glEnable(GL_TEXTURE_2D);
            glClearColor(0, 0, 0, 1.0f);
            glClearDepth(1);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            glMatrixMode(GL_MODELVIEW);
            glEnable(GL_CULL_FACE);  // backface culling
        }

        static void view3d(int Width, int Height)
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            Glu.gluPerspective(45, Width / (double)Height, 0.1, 100);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }

        static void Viewport(Rect rect)
        {
            glViewport(rect.Left, rect.Top, rect.Width, rect.Height);
        }

        static void view2d(int Width, int Height)
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, Width, Height, 0, 0, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }
    }
}
