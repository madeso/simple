#include "engine/main.h"

#include <memory>

#include "demo/demo.h"
#include "engine/setup.h"

namespace SimpleTest
{
    struct App : public SimpleEngine::App
    {
        std::shared_ptr<Demo> demo;
        SimpleEngine::vec2 mouse_delta = SimpleEngine::vec2(0, 0);

        App()
        {
            SimpleEngine::Setup::Setup();
            demo = std::make_shared<Demo>();
        }

        void OnStep(float dt) override
        {
            demo->OnFrame(dt, mouse_delta);
            mouse_delta = SimpleEngine::vec2(0, 0);
        }

        void OnEvent(const SDL_Event& e) override
        {
            if (use_mouse)
            {
                switch (e.type)
                {
                case SDL_MOUSEMOTION:
                    mouse_delta = mouse_delta + SimpleEngine::vec2(e.motion.xrel, e.motion.yrel);
                    break;
                }
            }

            if (use_keyboard)
            {
                switch (e.type)
                {
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    demo->OnButton(SimpleEngine::Ckey(e.key.keysym.sym), e.type == SDL_KEYDOWN);
                    break;
                }
            }
        }

        void OnRender(int w, int h) override
        {
            demo->Width = w;
            demo->Height = h;
            demo->OnBeginRender();
            demo->Render();
        }
    };
}

int main(int argc, char* argv[])
{
    SimpleEngine::RunMain([] { return std::make_shared<SimpleTest::App>(); });
    return 0;
}
