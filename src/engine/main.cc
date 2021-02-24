#include "engine/main.h"

#include <SDL.h>

#include "engine/opengl.h"
#include "engine/vec3.h"
#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl.h"

namespace SimpleEngine
{
    void RunMain(std::function<std::shared_ptr<App>()>&& make_app)
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        static const int width = 800;
        static const int height = 600;

        auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;

        SDL_Window* window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        SDL_GLContext context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, context);
        SDL_GL_SetSwapInterval(1);

        if (gladLoadGL() == 0)
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to init opengl\n");
            return;
        }

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        ImGui_ImplOpenGL2_Init();
        ImGui_ImplSDL2_InitForOpenGL(window, context);

        // todo(Gustav): filebrowser doesn't support light colors...
        // ImGui::StyleColorsLight();
        ImGui::StyleColorsDark();

        auto app = make_app();

        Uint64 now = SDL_GetPerformanceCounter();
        Uint64 last = now;

        while (app->run)
        {
            last = now;
            now = SDL_GetPerformanceCounter();
            const auto dt = static_cast<float>((now - last)) / static_cast<float>(SDL_GetPerformanceFrequency());

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL2_ProcessEvent(&event);
                app->use_mouse = !io.WantCaptureMouse;
                app->use_keyboard = !io.WantCaptureKeyboard;
                app->OnEvent(event);
                switch (event.type)
                {
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        app->run = false;
                    }
                    break;
                }
            }

            ImGui_ImplOpenGL2_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();

            app->OnStep(dt);

            ImGui::Render();
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClear(GL_COLOR_BUFFER_BIT);
            // glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be
            // bound

            app->OnRender((int)io.DisplaySize.x, (int)io.DisplaySize.y);
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

            SDL_GL_SwapWindow(window);
        }

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}
