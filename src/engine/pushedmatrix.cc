#include "engine/pushedmatrix.h"

#include "engine/opengl.h"

namespace simple
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
