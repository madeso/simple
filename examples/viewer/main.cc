#include "engine/main.h"

#include "engine/setup.h"
#include "imgui.h"
#include "viewer/viewer.h"

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
