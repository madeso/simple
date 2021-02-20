#include "engine/main.h"

#include "engine/setup.h"
#include "imgui.h"
#include "viewer.h"

namespace ModelView
{
    struct App : public SimpleEngine::App
    {
        Viewer viewer;

        App()
        {
            SimpleEngine::Setup::Setup();
        }

        void OnStep() override
        {
            ImGui::Begin("viewer");
            ImGui::End();
        }

        void OnEvent(const SDL_Event& e) override
        {
            switch (e.type)
            {
            case SDL_MOUSEMOTION:
                viewer.MouseMove(e.motion.x, e.motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                viewer.OnLMB(e.button.x, e.button.y, e.type == SDL_MOUSEBUTTONDOWN);
                break;
            case SDL_MOUSEWHEEL:
                viewer.OnMouseWheel(e.wheel.y);
                break;
            }
        }

        void OnRender(int w, int h) override
        {
            SimpleEngine::Setup::BeforeRender(w, h);
            viewer.Paint();
        }
    };
}

int main(int argc, char* argv[])
{
    SimpleEngine::RunMain([] { return std::make_shared<ModelView::App>(); });

    return 0;
}
