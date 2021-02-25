#include "engine/fullscreenquad.h"

#include "engine/fbo.h"
#include "engine/opengl.h"
#include "engine/setup.h"
#include "engine/texture.h"

namespace SimpleEngine
{
    void RenderFullscreenQuad(std::shared_ptr<Fbo> texture, int Width, int Height)
    {
        Setup::view2d(Width, Height);

        if (texture != nullptr)
        {
            texture->bindTexture();
        }

        glBegin(GL_QUADS);
        glTexCoord2f(0, 1);
        glVertex2f(0, 0);  // top left
        glTexCoord2f(0, 0);
        glVertex2f(0, Height);  // bottom left
        glTexCoord2f(1, 0);
        glVertex2f(Width, Height);  // bottom right
        glTexCoord2f(1, 1);
        glVertex2f(Width, 0);  // top right
        glEnd();
    }
}
