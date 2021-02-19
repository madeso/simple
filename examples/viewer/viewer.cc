using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Tao.OpenGl;
using SimpleEngine;
using System.IO;

namespace ModelView
{
    struct Main : Form
    {
        Main()
        {
            InitializeComponent();
            dView.paint = dView_Paint;

            dView.MouseWheel += MouseEventHandler(dView_MouseWheel);
            dView.dgl.MouseMove += MouseEventHandler(dView_MouseMove);
            dView.dgl.MouseDown += MouseEventHandler(dView_MouseDown);
            dView.dgl.MouseUp += MouseEventHandler(dView_MouseUp);
        }

        void Main_FormClosed(object sender, FormClosedEventArgs e)
        {
            dView.Kill();
        }

        void selectMesh()
        {
            if (dImportDialog.ShowDialog() == DialogResult.OK)
            {
                std::string basefile = Path.GetDirectoryName(dImportDialog.FileName);
                std::string filename = Path.GetFileName(dImportDialog.FileName);
                FileSystem fs = FileSystem();
                fs.addRoot(basefile);

                dAnimations.DropDownItems.Clear();

                ToolStripButton btn = ToolStripButton("Add animations");
                btn.Click += EventHandler(btn_Click);
                dAnimations.DropDownItems.Add(btn);

                std::string extent = Path.GetExtension(filename);
                if (extent == ".act")
                {
                    anim = nullptr;

                    Actor act = ActorFile.Load(fs, filename);
                    def = act.Mesh;
                    mesh = nullptr;

                    for (KeyValuePair<std::string, Animation> an : act.Animations)
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

                dModelActions.Text = filename;

                dMeshInfo.Text = fmt::format("{0} points, {1} texcoords {2} tris, {3}/{4} bones", def.points.Count, def.uvs.Count, def.TriCount, def.bones.Count, std::vector<MeshDef.Bone>(def.RootBones).Count);

                forceRedraw();
            }
        }

        void newMesh(FileSystem fs)
        {
            MediaLoader ml = MediaLoader(fs);
            def.compile(ml);
            mesh = def.Compiled;
            forceRedraw();
        }

        void btn_Click(object sender, EventArgs e)
        {
            selectAnimation();
        }

        MeshDef def = nullptr;
        CompiledMesh mesh = nullptr;
        Animation anim = nullptr;
        Rotation rotation = EasyRotation();

        void dView_Paint()
        {
            vec3 c = vec3::In() * distance;
            glTranslatef(c.x, c.y, c.z);
            rotation.rotateGl();

            RenderList list = RenderList();

            RenderableGrid grid = RenderableGrid();
            grid.sendToRenderer(list);

            if (mesh != nullptr)
            {
                mesh.sendToRenderer(list, vec3::Zero(), quat::Identity());
            }
            list.render();
        }

        void dView_MouseMove(object sender, MouseEventArgs e)
        {
            if (down)
            {
                vec2 current = vec2(e.X, e.Y);
                rotation.sendMouse(current, oldmouse);
                oldmouse = current;
            }
            forceRedraw();
        }

        void dView_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                down = false;
            }
        }

        vec2 oldmouse = vec2.Zero;
        bool down = false;

        void dView_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                oldmouse = vec2(e.X, e.Y);
                down = true;
            }
        }

        int zoommem = 0;
        const int kWheelStep = 120;
        float distance = 15;
        void dView_MouseWheel(object sender, MouseEventArgs e)
        {
            zoommem += e.Delta;
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
            dView.Draw();
        }

        void normalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            arcBallToolStripMenuItem.Checked = false;
            normalToolStripMenuItem.Checked = true;
            rotation = BasicQuatRot();
        }

        void arcBallToolStripMenuItem_Click(object sender, EventArgs e)
        {
            arcBallToolStripMenuItem.Checked = true;
            normalToolStripMenuItem.Checked = false;
            rotation = ArcBallRotation(this);
        }

        void selectAnimation()
        {
            if (dSelectAnimation.ShowDialog() == DialogResult.OK)
            {
                std::string basefile = Path.GetDirectoryName(dSelectAnimation.FileName);
                std::string filename = Path.GetFileName(dSelectAnimation.FileName);
                FileSystem fs = FileSystem();
                fs.addRoot(basefile);
                addAnimation(Path.GetFileNameWithoutExtension(filename), AnimationFile.Load(fs, filename));
            }
        }

        void addAnimation(std::string name, Animation anim)
        {
            ToolStripButton selectanim = ToolStripButton(name);
            selectanim.Tag = anim;
            selectanim.Click += EventHandler(selectAnimationClicked);
            dAnimations.DropDownItems.Add(selectanim);
            setAnimation(anim, name);
        }

        void setAnimation(Animation anim, std::string name)
        {
            this.anim = anim;
            dAnimation.Value = 0;
            dAnimations.Text = name;

            dAnimInfo.Text = fmt::format("{0} bones, {1}s long", anim.bones.Count, anim.Length);

            updatePose();
        }

        void selectAnimationClicked(object sender, EventArgs e)
        {
            ToolStripButton b = (ToolStripButton)sender;
            Animation an = (Animation)b.Tag;
            setAnimation(an, b.Text);
        }

        void updatePose()
        {
            if (anim == nullptr)
                return;
            if (def == nullptr)
                return;
            if (mesh == nullptr)
                return;
            float val = dAnimation.Value / (float)dAnimation.Maximum;
            Pose pose = anim.getPose(anim.Length * val);
            mesh.setPose(CompiledPose.Compile(pose, def));
            forceRedraw();
        }

        void dAnimation_ValueChanged(object sender, EventArgs e)
        {
            updatePose();
        }

        void importToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            selectMesh();
        }

        void exportToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        void dPlayPause_ButtonClick(object sender, EventArgs e)
        {
            if (dAnimTimer.Enabled)
                dAnimTimer.Stop();
            else
                dAnimTimer.Start();
        }

        void playPauseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dAnimTimer.Enabled)
                dAnimTimer.Stop();
        }

        void endToolStripMenuItem_Click(object sender, EventArgs e)
        {
            dAnimation.Value = dAnimation.Maximum;
        }

        void beginToolStripMenuItem_Click(object sender, EventArgs e)
        {
            dAnimation.Value = 0;
        }

        void dModelActions_ButtonClick(object sender, EventArgs e)
        {
            selectMesh();
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

        float AnimationPositionPercent
        {
            get
            {
                return dAnimation.Value / (float)dAnimation.Maximum;
            }
            set
            {
                dAnimation.Value = (int)(dAnimation.Maximum * value);
            }
        }

        float AnimationPositionSec
        {
            get
            {
                return anim.Length * AnimationPositionPercent;
            }
            set
            {
                AnimationPositionPercent = value / anim.Length;
            }
        }

        void dAnimTimer_Tick(object sender, EventArgs e)
        {
            if (anim == nullptr)
                return;
            if (def == nullptr)
                return;
            if (mesh == nullptr)
                return;

            float change = dAnimTimer.Interval / 100.0f;
            float nv = AnimationPositionSec + change;
            while (nv > anim.Length) nv -= anim.Length;
            AnimationPositionSec = nv;
            forceRedraw();
        }
    }
}
