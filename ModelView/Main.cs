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
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
            dView.paint = dView_Paint;

            dView.MouseWheel += new MouseEventHandler(dView_MouseWheel);
            dView.dgl.MouseMove += new MouseEventHandler(dView_MouseMove);
            dView.dgl.MouseDown += new MouseEventHandler(dView_MouseDown);
            dView.dgl.MouseUp += new MouseEventHandler(dView_MouseUp);
        }

        private void Main_FormClosed(object sender, FormClosedEventArgs e)
        {
            dView.Kill();
        }

        private void selectMesh()
        {
            if (dImportDialog.ShowDialog() == DialogResult.OK)
            {
                string basefile = Path.GetDirectoryName(dImportDialog.FileName);
                string filename = Path.GetFileName(dImportDialog.FileName);
                FileSystem fs = new FileSystem();
                fs.addRoot(basefile);

                dAnimations.DropDownItems.Clear();

                ToolStripButton btn = new ToolStripButton("Add animations");
                btn.Click += new EventHandler(btn_Click);
                dAnimations.DropDownItems.Add(btn);

                string extent = Path.GetExtension(filename);
                if (extent == ".act")
                {
                    anim = null;

                    Actor act = ActorFile.Load(fs, filename);
                    def = act.Mesh;
                    mesh = null;

                    foreach (KeyValuePair<string, Animation> an in act.Animations)
                    {
                        addAnimation(an.Key, an.Value);
                    }
                }
                else
                {
                    def = MeshFile.Load(fs, filename);
                    anim = null;
                }

                newMesh(fs);
                updatePose();

                dModelActions.Text = filename;

                dMeshInfo.Text = string.Format("{0} points, {1} texcoords {2} tris, {3}/{4} bones", def.points.Count, def.uvs.Count, def.TriCount, def.bones.Count, new List<MeshDef.Bone>(def.RootBones).Count);

                forceRedraw();
            }
        }

        private void newMesh(FileSystem fs)
        {
            MediaLoader ml = new MediaLoader(fs);
            def.compile(ml);
            mesh = def.Compiled;
            forceRedraw();
        }

        void btn_Click(object sender, EventArgs e)
        {
            selectAnimation();
        }

        MeshDef def = null;
        CompiledMesh mesh = null;
        Animation anim = null;
        Rotation rotation = new EasyRotation();

        private void dView_Paint()
        {
            vec3 c = vec3.In * distance;
            Gl.glTranslatef(c.x, c.y, c.z);
            rotation.rotateGl();

            RenderList list = new RenderList();

            RenderableGrid grid = new RenderableGrid();
            grid.sendToRenderer(list);
            
            if (mesh != null)
            {
                mesh.sendToRenderer(list, vec3.Zero, quat.Identity);
            }
            list.render();
        }

        private void dView_MouseMove(object sender, MouseEventArgs e)
        {
            if (down)
            {
                vec2 current = new vec2(e.X, e.Y);
                rotation.sendMouse(current, oldmouse);
                oldmouse = current;
            }
            forceRedraw();
        }

        private void dView_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                down = false;
            }
        }


        vec2 oldmouse = vec2.Zero;
        bool down = false;

        private void dView_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                oldmouse = new vec2(e.X, e.Y);
                down = true;
            }
        }

        private int zoommem = 0;
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
                if (distance < 0) distance = 0;
                forceRedraw();
            }
        }

        private void forceRedraw()
        {
            dView.Draw();
        }

        private void normalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            arcBallToolStripMenuItem.Checked = false;
            normalToolStripMenuItem.Checked = true;
            rotation = new BasicQuatRot();
        }

        private void arcBallToolStripMenuItem_Click(object sender, EventArgs e)
        {
            arcBallToolStripMenuItem.Checked = true;
            normalToolStripMenuItem.Checked = false;
            rotation = new ArcBallRotation(this);
        }

        private void selectAnimation()
        {
            if (dSelectAnimation.ShowDialog() == DialogResult.OK)
            {
                string basefile = Path.GetDirectoryName(dSelectAnimation.FileName);
                string filename = Path.GetFileName(dSelectAnimation.FileName);
                FileSystem fs = new FileSystem();
                fs.addRoot(basefile);
                addAnimation(Path.GetFileNameWithoutExtension(filename), AnimationFile.Load(fs, filename));
            }
        }

        private void addAnimation(string name, Animation anim)
        {
            ToolStripButton selectanim = new ToolStripButton(name);
            selectanim.Tag = anim;
            selectanim.Click += new EventHandler(selectAnimationClicked);
            dAnimations.DropDownItems.Add(selectanim);
            setAnimation(anim, name);
        }

        private void setAnimation(Animation anim, string name)
        {
            this.anim = anim;
            dAnimation.Value = 0;
            dAnimations.Text = name;

            dAnimInfo.Text = string.Format("{0} bones, {1}s long", anim.bones.Count, anim.Length);

            updatePose();
        }

        void selectAnimationClicked(object sender, EventArgs e)
        {
            ToolStripButton b = (ToolStripButton)sender;
            Animation an = (Animation)b.Tag;
            setAnimation(an, b.Text);
        }

        private void updatePose()
        {
            if (anim == null) return;
            if( def == null ) return;
            if( mesh == null ) return;
            float val = dAnimation.Value / (float)dAnimation.Maximum;
            Pose pose = anim.getPose(anim.Length * val);
            mesh.setPose( CompiledPose.Compile(pose,def) );
            forceRedraw();
        }

        private void dAnimation_ValueChanged(object sender, EventArgs e)
        {
            updatePose();
        }

        private void importToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            selectMesh();
        }

        private void exportToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void dPlayPause_ButtonClick(object sender, EventArgs e)
        {
            if (dAnimTimer.Enabled) dAnimTimer.Stop();
            else dAnimTimer.Start();
        }

        private void playPauseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dAnimTimer.Enabled) dAnimTimer.Stop();
        }

        private void endToolStripMenuItem_Click(object sender, EventArgs e)
        {
            dAnimation.Value = dAnimation.Maximum;
        }

        private void beginToolStripMenuItem_Click(object sender, EventArgs e)
        {
            dAnimation.Value = 0;
        }

        private void dModelActions_ButtonClick(object sender, EventArgs e)
        {
            selectMesh();
        }

        private void selectMaterialToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeMaterial mat = new ChangeMaterial(def);
            if (mat.ShowDialog() == DialogResult.OK)
            {
                def.getMaterialNamed(mat.MaterialName).texture = Path.GetFileName(mat.Texture);
                FileSystem fs = new FileSystem();
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

        private void dAnimTimer_Tick(object sender, EventArgs e)
        {
            if (anim == null) return;
            if (def == null) return;
            if (mesh == null) return;

            float change = dAnimTimer.Interval / 100.0f;
            float nv = AnimationPositionSec + change;
            while( nv > anim.Length ) nv -= anim.Length;
            AnimationPositionSec = nv;
            forceRedraw();
        }
    }
}
