#include "engine/pushedmatrix.h"

#include "engine/opengl.h"

namespace SimpleEngine
{
    PushedMatrix::PushedMatrix()
    {
        glPushMatrix();
    }

    PushedMatrix::~PushedMatrix()
    {
        glPopMatrix();
    }
}
