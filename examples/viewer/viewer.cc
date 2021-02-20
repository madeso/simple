#include <memory>
#include <string>

#include "engine/animation.h"
#include "engine/compiledmesh.h"
#include "engine/filesystem.h"
#include "engine/medialoader.h"
#include "engine/meshdef.h"
#include "engine/opengl.h"
#include "engine/renderablegrid.h"
#include "engine/renderlist.h"
#include "engine/rotation.h"
#include "engine/window.h"
#include "fmt/core.h"

using namespace SimpleEngine;

namespace ModelView
{
    struct Main : public Window
    {
        std::string current_filename;
        std::string model_information;

        std::shared_ptr<MeshDef> def;
        std::shared_ptr<CompiledMesh> mesh;
        std::shared_ptr<Animation> anim;
        std::shared_ptr<Rotation> rotation = std::make_shared<EasyRotation>();

        vec2 oldmouse = vec2::Zero();
        bool down = false;
        int zoommem = 0;
        const int kWheelStep = 120;
        float distance = 15;

        float animation_position = 0;
        bool playing_animation = false;

        Main()
        {
        }

        void selectMesh(const std::string& fileName)
        {
            std::string basefile = Path.GetDirectoryName(dImportDialog.FileName);
            std::string filename = Path.GetFileName(dImportDialog.FileName);

            std::shared_ptr<FileSystem> fs = std::make_shared<FileSystem>();
            fs->addRoot(basefile);

            std::string extent = Path.GetExtension(filename);
            if (extent == ".act")
            {
                anim = nullptr;

                Actor act = ActorFile.Load(fs, filename);
                def = act.Mesh;
                mesh = nullptr;

                for (auto& an : act.Animations)
                {
                    addAnimation(an.Key, an.Value);
                }
            }
            else
            {
                def = MeshFile.Load(fs, filename);
                anim = nullptr;
            }

            newMesh(fs);
            updatePose();

            current_filename = filename;
            model_information = fmt::format("{0} points, {1} texcoords {2} tris, {3}/{4} bones", def.points.Count, def.uvs.Count, def.TriCount, def.bones.Count, std::vector<Bone>(def.RootBones).Count);

            forceRedraw();
        }

        void newMesh(std::shared_ptr<FileSystem> fs)
        {
            auto ml = MediaLoader(fs);
            def->compile(ml);
            mesh = def->Compiled();
            forceRedraw();
        }

        void Paint()
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

        void MouseMove(float mx, float my)
        {
            if (down)
            {
                vec2 current = vec2(mx, my);
                rotation->sendMouse(current, oldmouse);
                oldmouse = current;
            }
            forceRedraw();
        }

        void OnLMB(float mx, float my, bool state)
        {
            if (state && !down)
            {
                oldmouse = vec2(mx, my);
            }
            down = state;
        }

        void OnMouseWheel(int delta)
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

        void forceRedraw()
        {
        }

        void SetBasicCamera()
        {
            rotation = std::make_shared<BasicQuatRot>();
        }

        void SetArcballCamera()
        {
            rotation = std::make_shared<ArcBallRotation>(this);
        }

        void selectAnimation(const std::string& fileName)
        {
            std::string basefile = Path.GetDirectoryName(dSelectAnimation.FileName);
            std::string filename = Path.GetFileName(dSelectAnimation.FileName);
            FileSystem fs = FileSystem();
            fs.addRoot(basefile);
            addAnimation(Path.GetFileNameWithoutExtension(filename), AnimationFile.Load(fs, filename));
        }

        void addAnimation(const std::string& name, std::shared_ptr<Animation> anim)
        {
            ToolStripButton selectanim = ToolStripButton(name);
            selectanim.Tag = anim;
            selectanim.Click += EventHandler(selectAnimationClicked);
            dAnimations.DropDownItems.Add(selectanim);
            setAnimation(anim, name);
        }

        std::string animation_information;

        void setAnimation(std::shared_ptr<Animation> anim, const std::string& name)
        {
            this->anim = anim;
            dAnimation.Value = 0;
            dAnimations.Text = name;

            animation_information = fmt::format("{0} bones, {1}s long", anim->bones.size(), anim->Length());

            updatePose();
        }

        float SafeAnimationPosition()
        {
            if (animation_position < 0.0f)
            {
                return 0.0f;
            }

            if (anim == nullptr)
            {
                return;
            }

            const auto l = anim->Length();
            if (animation_position > l)
            {
                return l;
            }
            else
                return animation_position;
        }

        void updatePose()
        {
            if (anim == nullptr)
                return;
            if (def == nullptr)
                return;
            if (mesh == nullptr)
                return;
            float val = SafeAnimationPosition();
            auto pose = anim->getPose(val);
            mesh->setPose(CompiledPose::Compile(pose, def));
            forceRedraw();
        }

        void selectMaterialToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeMaterial mat = ChangeMaterial(def);
            if (mat.ShowDialog() == DialogResult.OK)
            {
                def.getMaterialNamed(mat.MaterialName).texture = Path.GetFileName(mat.Texture);
                FileSystem fs = FileSystem();
                fs.addRoot(Path.GetDirectoryName(mat.Texture));
                newMesh(fs);
            }
        }

        void step(float dt)
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

            while (animation_position > anim->Length())
            {
                animation_position -= anim->Length();
            }
            updatePose();
            forceRedraw();
        }
    }
}
