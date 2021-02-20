#include "engine/main.h"

#include "imgui.h"

struct App : public SimpleEngine::App
{
    void OnStep() override
    {
        ImGui::Begin("Hello, world!");
        ImGui::End();
    }

    void OnRender() override
    {
    }
};

int main(int argc, char* argv[])
{
    SimpleEngine::RunMain([] { return std::make_shared<App>(); });

    return 0;
}
