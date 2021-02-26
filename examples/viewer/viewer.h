#include <map>
#include <memory>
#include <string>

#include "engine/vec2.h"
#include "engine/window.h"

namespace simple
{
    struct CompiledMesh;
    struct MeshDef;
    struct Animation;
    struct Rotation;
    struct FileSystem;
}

namespace viewer
{
    struct Viewer : public simple::Window
    {
        std::string current_filename;
        std::string model_information;
        std::shared_ptr<simple::MeshDef> def;
        std::shared_ptr<simple::CompiledMesh> mesh;
        std::shared_ptr<simple::Animation> anim;
        std::shared_ptr<simple::Rotation> rotation;
        simple::vec2 old_mouse;
        bool down = false;
        int zoom_memory = 0;
        const int kWheelStep = 1;
        float distance = 15;
        float animation_position = 0;
        bool playing_animation = false;
        std::map<std::string, std::shared_ptr<simple::Animation>> animations;
        std::string animation_information;
        std::string current_animation;

        bool using_easy_camera = false;
        bool using_basic_camera = false;
        bool using_arcball_camera = false;

        Viewer();
        ~Viewer();

        void LoadMeshFromFile(const std::string& fileName);

        void newMesh(std::shared_ptr<simple::FileSystem> fs);

        void OnRender();

        void OnMouseMove(float mx, float my);
        void OnLMB(float mx, float my, bool state);
        void OnMouseWheel(int delta);

        void forceRedraw();

        void ClearCamera();
        void SetEasyCamera();
        void SetBasicCamera();
        void SetArcballCamera();

        void selectAnimation(const std::string& fileName);
        void addAnimation(const std::string& name, std::shared_ptr<simple::Animation> anim);
        void SelectLoadedAnimation(std::shared_ptr<simple::Animation> anim, const std::string& name);
        float SafeAnimationPosition();
        void UpdatePose();
        float GetMaxTimeForCurrentAnimation();

        void OnStep(float dt);
    };
}
