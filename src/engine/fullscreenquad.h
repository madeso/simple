#pragma once
#include "engine/fbo.h"

namespace SimpleEngine::FullscreenQuad
{
    void render(std::shared_ptr<Fbo> texture, int Width, int Height);
}
