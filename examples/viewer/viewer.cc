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

using namespace simple;

namespace viewer
{
    Viewer::Viewer()
    {
        SetEasyCamera();
    }

    Viewer::~Viewer()
    {
    }

    void Viewer::LoadMeshFromFile(const std::string& fileName)
    {
        auto [basefile, filename] = file_util::Split(fileName);

        std::shared_ptr<FileSystem> fs = std::make_shared<FileSystem>();
        fs->AddRoot(basefile);

        std::string extent = file_util::GetExtension(filename);
        if (extent == ".act")
        {
            anim = nullptr;

            auto act = actor_file::Load(fs.get(), filename);
            def = act->mesh;
            mesh = nullptr;

            for (auto& an : act->animations)
            {
                addAnimation(an.first, an.second);
            }
        }
        else
        {
            def = mesh_file::Load(fs.get(), filename);
            anim = nullptr;
        }

        newMesh(fs);
        UpdatePose();

        current_filename = filename;
        model_information = fmt::format("{0} points, {1} texcoords {2} tris, {3}/{4} bones",
                                        def->points.size(), def->texturecoordinates.size(), def->GetTriangleCount(), def->bones.size(), def->GetRootBones().size());

        forceRedraw();
    }

    void Viewer::newMesh(std::shared_ptr<FileSystem> fs)
    {
        auto ml = MediaLoader(fs);
        def->Compile(&ml);
        mesh = def->GetCompiledMesh();
        forceRedraw();
    }

    void Viewer::OnRender()
    {
        vec3 c = vec3::In() * distance;
        glTranslatef(c.x, c.y, c.z);
        rotation->RotateOpenGl();

        RenderList list;

        RenderableGrid grid;
        grid.SendToRenderer(&list);

        if (mesh != nullptr)
        {
            mesh->SendToRenderer(&list, vec3::Zero(), quat::Identity());
        }

        list.OnRender();
    }

    void Viewer::OnMouseMove(float mx, float my)
    {
        if (down)
        {
            vec2 current = vec2(mx, my);
            rotation->SendMouse(current, old_mouse);
            old_mouse = current;
        }
        forceRedraw();
    }

    void Viewer::OnLMB(float mx, float my, bool state)
    {
        if (state && !down)
        {
            old_mouse = vec2(mx, my);
        }
        down = state;
    }

    void Viewer::OnMouseWheel(int delta)
    {
        zoom_memory += delta;
        int zoommove = 0;

        while (zoom_memory >= kWheelStep)
        {
            zoom_memory -= kWheelStep;
            ++zoommove;
        }
        while (zoom_memory <= -kWheelStep)
        {
            zoom_memory += kWheelStep;
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
        rotation = std::make_shared<BasicQuatRotation>();
    }

    void Viewer::SetArcballCamera()
    {
        ClearCamera();
        using_arcball_camera = true;
        rotation = std::make_shared<ArcBallRotation>(this);
    }

    void Viewer::selectAnimation(const std::string& fileName)
    {
        auto [basefile, filename] = file_util::Split(fileName);

        FileSystem fs = FileSystem();
        fs.AddRoot(basefile);
        addAnimation(file_util::GetFileNameWithoutExtension(filename), animation_file::Load(&fs, filename));
    }

    void Viewer::addAnimation(const std::string& name, std::shared_ptr<Animation> anim)
    {
        animations.emplace(name, anim);
        SelectLoadedAnimation(anim, name);
    }

    void Viewer::SelectLoadedAnimation(std::shared_ptr<Animation> anim, const std::string& name)
    {
        this->anim = anim;
        animation_position = 0.0f;
        current_animation = name;

        animation_information = fmt::format("{0} bones, {1}s long", anim->bones.size(), anim->length);

        UpdatePose();
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

        if (animation_position > anim->length)
        {
            return anim->length;
        }
        else
        {
            return animation_position;
        }
    }

    float Viewer::GetMaxTimeForCurrentAnimation()
    {
        if (anim == nullptr)
        {
            return 0.0f;
        }

        return anim->length;
    }

    void Viewer::UpdatePose()
    {
        if (anim == nullptr)
            return;
        if (def == nullptr)
            return;
        if (mesh == nullptr)
            return;
        float val = SafeAnimationPosition();
        auto pose = anim->GetPose(val);
        mesh->SetPose(std::make_shared<CompiledPose>(CompiledPose::Compile(pose, *def)));
        forceRedraw();
    }

    void Viewer::OnStep(float dt)
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

        while (animation_position > anim->length)
        {
            animation_position -= anim->length;
        }
        UpdatePose();
        forceRedraw();
    }
}
