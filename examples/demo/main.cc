#include "engine/main.h"

#include "engine/setup.h"

namespace SimpleTest
{
    struct App : public SimpleEngine::App
    {
        App()
        {
            SimpleEngine::Setup::Setup();
        }

        void OnStep(float dt) override
        {
        }

        void OnEvent(const SDL_Event& e) override
        {
        }

        void OnRender(int w, int h) override
        {
        }
    };
}

int main(int argc, char* argv[])
{
    SimpleEngine::RunMain([] { return std::make_shared<SimpleTest::App>(); });
    return 0;
}
