#include "engine/setup.h"

#include <cmath>

#include "math.h"
#include "engine/opengl.h"
#include "engine/rect.h"

namespace
{
    void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
                      float znear, float zfar)
    {
        float temp, temp2, temp3, temp4;
        temp = 2.0 * znear;
        temp2 = right - left;
        temp3 = top - bottom;
        temp4 = zfar - znear;
        matrix[0] = temp / temp2;
        matrix[1] = 0.0;
        matrix[2] = 0.0;
        matrix[3] = 0.0;
        matrix[4] = 0.0;
        matrix[5] = temp / temp3;
        matrix[6] = 0.0;
        matrix[7] = 0.0;
        matrix[8] = (right + left) / temp2;
        matrix[9] = (top + bottom) / temp3;
        matrix[10] = (-zfar - znear) / temp4;
        matrix[11] = -1.0;
        matrix[12] = 0.0;
        matrix[13] = 0.0;
        matrix[14] = (-temp * zfar) / temp4;
        matrix[15] = 0.0;
    }

    void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
                          float znear, float zfar)
    {
        float ymax, xmax;
        float temp, temp2, temp3, temp4;
        ymax = znear * tanf(fovyInDegrees * simple::math::pi / 360.0);
        // ymin = -ymax;
        // xmin = -ymax * aspectRatio;
        xmax = ymax * aspectRatio;
        glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
    }

    void gluPerspective(float fovyInDegrees, float aspectRatio,
                        float znear, float zfar)
    {
        float matrix[16];
        glhPerspectivef2(matrix, fovyInDegrees, aspectRatio, znear, zfar);
        glLoadMatrixf(matrix);
    }
}

namespace simple
{
    namespace setup
    {
        void Setup()
        {
            glClearColor(1, 1, 1, 1);
            glShadeModel(GL_SMOOTH);
            glEnable(GL_TEXTURE_2D);
            glClearColor(1, 1, 1, 1.0f);
            glClearDepth(1);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            glMatrixMode(GL_MODELVIEW);
            glEnable(GL_CULL_FACE);
        }

        void BeforeRender(int Width, int Height)
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45, Width / (double)Height, 0.1, 100);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glClear(GL_DEPTH_BUFFER_BIT);
        }

        void BasicOpenGL()
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

        void View3d(int Width, int Height)
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45, Width / (float)Height, 0.1, 100);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }

        void Viewport(const Rect &rect)
        {
            glViewport(rect.left, rect.top, rect.width, rect.height);
        }

        void View2d(int Width, int Height)
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, Width, Height, 0, 0, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }
    }
}
