#include "viewer/viewer.h"

#include <memory>
#include <string>

#include "engine/actorfile.h"
#include "engine/animation.h"
#include "engine/animationfile.h"
#include "engine/compiledmesh.h"
#include "engine/filesystem.h"
#include "engine/fileutil.h"
#include "engine/medialoader.h"
#include "engine/meshdef.h"
#include "engine/meshfile.h"
#include "engine/opengl.h"
#include "engine/renderablegrid.h"
#include "engine/renderlist.h"
#include "engine/rotation.h"
#include "engine/window.h"
#include "fmt/core.h"

using namespace SimpleEngine;

namespace ModelView
{
    Viewer::Viewer()
    {
        SetEasyCamera();
    }

    Viewer::~Viewer()
    {
    }

    void Viewer::selectMesh(const std::string& fileName)
    {
        auto [basefile, filename] = FileUtil::Split(fileName);

        std::shared_ptr<FileSystem> fs = std::make_shared<FileSystem>();
        fs->addRoot(basefile);

        std::string extent = FileUtil::GetExtension(filename);
        if (extent == ".act")
        {
            anim = nullptr;

            auto act = ActorFile::Load(fs.get(), filename);
            def = act->mesh;
            mesh = nullptr;

            for (auto& an : act->animations)
            {
                addAnimation(an.first, an.second);
            }
        }
        else
        {
            def = MeshFile::Load(fs.get(), filename);
            anim = nullptr;
        }

        newMesh(fs);
        updatePose();

        current_filename = filename;
        model_information = fmt::format("{0} points, {1} texcoords {2} tris, {3}/{4} bones",
                                        def->points.size(), def->uvs.size(), def->TriCount(), def->bones.size(), def->RootBones().size());

        forceRedraw();
    }

    void Viewer::newMesh(std::shared_ptr<FileSystem> fs)
    {
        auto ml = MediaLoader(fs);
        def->compile(&ml);
        mesh = def->Compiled();
        forceRedraw();
    }

    void Viewer::Paint()
    {
        vec3 c = vec3::In() * distance;
        glTranslatef(c.x, c.y, c.z);
        rotation->rotateGl();

        RenderList list;

        RenderableGrid grid;
        grid.sendToRenderer(&list);

        if (mesh != nullptr)
        {
            mesh->sendToRenderer(&list, vec3::Zero(), quat::Identity());
        }

        list.render();
    }

    void Viewer::MouseMove(float mx, float my)
    {
        if (down)
        {
            vec2 current = vec2(mx, my);
            rotation->sendMouse(current, oldmouse);
            oldmouse = current;
        }
        forceRedraw();
    }

    void Viewer::OnLMB(float mx, float my, bool state)
    {
        if (state && !down)
        {
            oldmouse = vec2(mx, my);
        }
        down = state;
    }

    void Viewer::OnMouseWheel(int delta)
    {
        zoommem += delta;
        int zoommove = 0;

        while (zoommem >= kWheelStep)
        {
            zoommem -= kWheelStep;
            ++zoommove;
        }
        while (zoommem <= -kWheelStep)
        {
            zoommem += kWheelStep;
            --zoommove;
        }

        if (zoommove != 0)
        {
            distance += zoommove * 0.25f;
            if (distance < 0)
                distance = 0;
            forceRedraw();
        }
    }

    void Viewer::forceRedraw()
    {
    }

    void Viewer::ClearCamera()
    {
        using_easy_camera = false;
        using_basic_camera = false;
        using_arcball_camera = false;
        rotation = nullptr;
    }

    void Viewer::SetEasyCamera()
    {
        ClearCamera();
        using_easy_camera = true;
        rotation = std::make_shared<EasyRotation>();
    }

    void Viewer::SetBasicCamera()
    {
        ClearCamera();
        using_basic_camera = true;
        rotation = std::make_shared<BasicQuatRot>();
    }

    void Viewer::SetArcballCamera()
    {
        ClearCamera();
        using_arcball_camera = true;
        rotation = std::make_shared<ArcBallRotation>(this);
    }

    void Viewer::selectAnimation(const std::string& fileName)
    {
        auto [basefile, filename] = FileUtil::Split(fileName);

        FileSystem fs = FileSystem();
        fs.addRoot(basefile);
        addAnimation(FileUtil::GetFileNameWithoutExtension(filename), AnimationFile::Load(&fs, filename));
    }

    void Viewer::addAnimation(const std::string& name, std::shared_ptr<Animation> anim)
    {
        animations.emplace(name, anim);
        setAnimation(anim, name);
    }

    void Viewer::setAnimation(std::shared_ptr<Animation> anim, const std::string& name)
    {
        this->anim = anim;
        animation_position = 0.0f;
        current_animation = name;

        animation_information = fmt::format("{0} bones, {1}s long", anim->bones.size(), anim->Length);

        updatePose();
    }

    float Viewer::SafeAnimationPosition()
    {
        if (animation_position < 0.0f)
        {
            return 0.0f;
        }

        if (anim == nullptr)
        {
            return animation_position;
        }

        if (animation_position > anim->Length)
        {
            return anim->Length;
        }
        else
        {
            return animation_position;
        }
    }

    float Viewer::getMaxAnimation()
    {
        if (anim == nullptr)
        {
            return 0.0f;
        }

        return anim->Length;
    }

    void Viewer::updatePose()
    {
        if (anim == nullptr)
            return;
        if (def == nullptr)
            return;
        if (mesh == nullptr)
            return;
        float val = SafeAnimationPosition();
        auto pose = anim->getPose(val);
        mesh->setPose(std::make_shared<CompiledPose>(CompiledPose::Compile(pose, *def)));
        forceRedraw();
    }

    void Viewer::step(float dt)
    {
        if (anim == nullptr)
            return;
        if (def == nullptr)
            return;
        if (mesh == nullptr)
            return;

        if (playing_animation == false)
        {
            return;
        }

        animation_position += dt;

        while (animation_position > anim->Length)
        {
            animation_position -= anim->Length;
        }
        updatePose();
        forceRedraw();
    }
}
