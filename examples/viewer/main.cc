#include "engine/main.h"

#include <iostream>

#include "ImGuiFileBrowser.h"
#include "engine/setup.h"
#include "imgui.h"
#include "viewer.h"

#define LOAD_MODEL "load-model"

namespace ModelView
{
    struct App : public SimpleEngine::App
    {
        Viewer viewer;
        imgui_addons::ImGuiFileBrowser file_dialog;
        bool demo = false;

        App()
        {
            SimpleEngine::Setup::Setup();
        }

        void OnStep() override
        {
            ImGui::Begin("viewer");
            ImGui::End();

            bool load_model = false;

            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Open model"))
                    {
                        load_model = true;
                    }
                    if (ImGui::MenuItem("Demo"))
                    {
                        demo = true;
                    }
                    if (ImGui::MenuItem("Quit"))
                    {
                        run = false;
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("View"))
                {
                    if (ImGui::MenuItem("Easy camera", "", &viewer.using_easy_camera))
                    {
                        viewer.SetEasyCamera();
                    }
                    if (ImGui::MenuItem("Basic camera", "", &viewer.using_basic_camera))
                    {
                        viewer.SetBasicCamera();
                    }
                    if (ImGui::MenuItem("Arcball camera", "", &viewer.using_arcball_camera))
                    {
                        viewer.SetArcballCamera();
                    }
                    // ImGui::Separator();
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            if (load_model)
            {
                ImGui::OpenPopup(LOAD_MODEL);
            }
            if (file_dialog.showFileDialog(LOAD_MODEL, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".act,.mdf,.obj,*.*"))
            {
                std::cout << file_dialog.selected_path << std::endl;  // The absolute path to the selected file
            }

            if (demo)
            {
                ImGui::ShowDemoWindow(&demo);
            }
        }

        void OnEvent(const SDL_Event& e) override
        {
            if (use_mouse)
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
