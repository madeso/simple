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

        void OnStep(float dt) override
        {
            viewer.step(dt);

            if (ImGui::Begin("viewer"))
            {
                if (dt > 0.0f)
                {
                    ImGui::Text("fps %f", 1 / dt);
                }

                ImGui::Text("%s", viewer.current_filename.c_str());
                ImGui::Text("%s", viewer.model_information.c_str());

                if (viewer.animations.empty() == false)
                {
                    ImGui::Checkbox("Play", &viewer.playing_animation);
                    if (ImGui::SliderFloat("Time", &viewer.animation_position, 0.0f, viewer.getMaxAnimation()))
                    {
                        viewer.updatePose();
                    }
                    if (ImGui::BeginCombo("Animation", viewer.current_animation.empty() ? "<select animation>" : viewer.current_animation.c_str()))
                    {
                        for (auto a : viewer.animations)
                        {
                            bool selected = false;
                            if (ImGui::Selectable(a.first.c_str(), &selected))
                            {
                                viewer.setAnimation(a.second, a.first);
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
            }
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
                viewer.selectMesh(file_dialog.selected_path);
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
            viewer.Width = w;
            viewer.Height = h;
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
