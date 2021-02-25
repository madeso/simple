#include <map>
#include <memory>
#include <string>

#include "engine/vec2.h"
#include "engine/window.h"

namespace SimpleEngine
{
    struct CompiledMesh;
    struct MeshDef;
    struct Animation;
    struct Rotation;
    struct FileSystem;
}

namespace ModelView
{
    struct Viewer : public SimpleEngine::Window
    {
        std::string current_filename;
        std::string model_information;
        std::shared_ptr<SimpleEngine::MeshDef> def;
        std::shared_ptr<SimpleEngine::CompiledMesh> mesh;
        std::shared_ptr<SimpleEngine::Animation> anim;
        std::shared_ptr<SimpleEngine::Rotation> rotation;
        SimpleEngine::vec2 oldmouse;
        bool down = false;
        int zoommem = 0;
        const int kWheelStep = 1;
        float distance = 15;
        float animation_position = 0;
        bool playing_animation = false;
        std::map<std::string, std::shared_ptr<SimpleEngine::Animation>> animations;
        std::string animation_information;
        std::string current_animation;

        bool using_easy_camera = false;
        bool using_basic_camera = false;
        bool using_arcball_camera = false;

        Viewer();
        ~Viewer();

        void selectMesh(const std::string& fileName);

        void newMesh(std::shared_ptr<SimpleEngine::FileSystem> fs);

        void Paint();

        void MouseMove(float mx, float my);
        void OnLMB(float mx, float my, bool state);
        void OnMouseWheel(int delta);

        void forceRedraw();

        void ClearCamera();
        void SetEasyCamera();
        void SetBasicCamera();
        void SetArcballCamera();

        void selectAnimation(const std::string& fileName);
        void addAnimation(const std::string& name, std::shared_ptr<SimpleEngine::Animation> anim);
        void setAnimation(std::shared_ptr<SimpleEngine::Animation> anim, const std::string& name);
        float SafeAnimationPosition();
        void updatePose();
        float getMaxAnimation();

        void step(float dt);
    };
}
