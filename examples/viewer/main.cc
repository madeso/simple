#include "engine/main.h"

#include "imgui.h"
#include "viewer/viewer.h"

namespace ModelView
{
    struct App : public SimpleEngine::App
    {
        Viewer viewer;

        void OnStep() override
        {
            ImGui::Begin("viewer");
            ImGui::End();
        }

        void OnRender() override
        {
            viewer.Paint();
        }
    };
}

int main(int argc, char* argv[])
{
    SimpleEngine::RunMain([] { return std::make_shared<ModelView::App>(); });

    return 0;
}
