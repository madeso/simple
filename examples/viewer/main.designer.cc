namespace ModelView
{
    struct Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System.ComponentModel.IContainer components = nullptr;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected
        override void Dispose(bool disposing)
        {
            if (disposing && (components != nullptr))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

#region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent()
        {
            this.components = System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.dStrip = System.Windows.Forms.StatusStrip();
            this.dModelActions = System.Windows.Forms.ToolStripSplitButton();
            this.importToolStripMenuItem1 = System.Windows.Forms.ToolStripMenuItem();
            this.exportToolStripMenuItem = System.Windows.Forms.ToolStripMenuItem();
            this.selectMaterialToolStripMenuItem = System.Windows.Forms.ToolStripMenuItem();
            this.toolStripDropDownButton1 = System.Windows.Forms.ToolStripDropDownButton();
            this.arcBallToolStripMenuItem = System.Windows.Forms.ToolStripMenuItem();
            this.normalToolStripMenuItem = System.Windows.Forms.ToolStripMenuItem();
            this.dPlayPause = System.Windows.Forms.ToolStripSplitButton();
            this.beginToolStripMenuItem = System.Windows.Forms.ToolStripMenuItem();
            this.endToolStripMenuItem = System.Windows.Forms.ToolStripMenuItem();
            this.playPauseToolStripMenuItem = System.Windows.Forms.ToolStripMenuItem();
            this.dAnimations = System.Windows.Forms.ToolStripDropDownButton();
            this.dMeshInfo = System.Windows.Forms.ToolStripStatusLabel();
            this.dAnimInfo = System.Windows.Forms.ToolStripStatusLabel();
            this.dImportDialog = System.Windows.Forms.OpenFileDialog();
            this.dAnimation = System.Windows.Forms.TrackBar();
            this.dSelectAnimation = System.Windows.Forms.OpenFileDialog();
            this.dAnimTimer = System.Windows.Forms.Timer(this.components);
            this.dView = ModelView.GlDraw();
            this.dStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dAnimation)).BeginInit();
            this.SuspendLayout();
            //
            // dStrip
            //
            this.dStrip.Items.AddRange(System.Windows.Forms.ToolStripItem[]{
                this.dModelActions,
                this.toolStripDropDownButton1,
                this.dPlayPause,
                this.dAnimations,
                this.dMeshInfo,
                this.dAnimInfo});
            this.dStrip.Location = System.Drawing.Point(0, 450);
            this.dStrip.Name = "dStrip";
            this.dStrip.Size = System.Drawing.Size(732, 24);
            this.dStrip.TabIndex = 2;
            this.dStrip.Text = "statusStrip1";
            //
            // dModelActions
            //
            this.dModelActions.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.dModelActions.DropDownItems.AddRange(System.Windows.Forms.ToolStripItem[]{
                this.importToolStripMenuItem1,
                this.exportToolStripMenuItem,
                this.selectMaterialToolStripMenuItem});
            this.dModelActions.Image = ((System.Drawing.Image)(resources.GetObject("dModelActions.Image")));
            this.dModelActions.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.dModelActions.Name = "dModelActions";
            this.dModelActions.Size = System.Drawing.Size(92, 22);
            this.dModelActions.Text = "Load model";
            this.dModelActions.ButtonClick += System.EventHandler(this.dModelActions_ButtonClick);
            //
            // importToolStripMenuItem1
            //
            this.importToolStripMenuItem1.Name = "importToolStripMenuItem1";
            this.importToolStripMenuItem1.Size = System.Drawing.Size(185, 22);
            this.importToolStripMenuItem1.Text = "Import...";
            this.importToolStripMenuItem1.Click += System.EventHandler(this.importToolStripMenuItem1_Click);
            //
            // exportToolStripMenuItem
            //
            this.exportToolStripMenuItem.Name = "exportToolStripMenuItem";
            this.exportToolStripMenuItem.Size = System.Drawing.Size(185, 22);
            this.exportToolStripMenuItem.Text = "Export...";
            this.exportToolStripMenuItem.Click += System.EventHandler(this.exportToolStripMenuItem_Click);
            //
            // selectMaterialToolStripMenuItem
            //
            this.selectMaterialToolStripMenuItem.Name = "selectMaterialToolStripMenuItem";
            this.selectMaterialToolStripMenuItem.Size = System.Drawing.Size(185, 22);
            this.selectMaterialToolStripMenuItem.Text = "Select Material...";
            this.selectMaterialToolStripMenuItem.Click += System.EventHandler(this.selectMaterialToolStripMenuItem_Click);
            //
            // toolStripDropDownButton1
            //
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripDropDownButton1.DropDownItems.AddRange(System.Windows.Forms.ToolStripItem[]{
                this.arcBallToolStripMenuItem,
                this.normalToolStripMenuItem});
            this.toolStripDropDownButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton1.Image")));
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = System.Drawing.Size(118, 22);
            this.toolStripDropDownButton1.Text = "Rotation method";
            //
            // arcBallToolStripMenuItem
            //
            this.arcBallToolStripMenuItem.Name = "arcBallToolStripMenuItem";
            this.arcBallToolStripMenuItem.Size = System.Drawing.Size(131, 22);
            this.arcBallToolStripMenuItem.Text = "Arc Ball";
            this.arcBallToolStripMenuItem.Click += System.EventHandler(this.arcBallToolStripMenuItem_Click);
            //
            // normalToolStripMenuItem
            //
            this.normalToolStripMenuItem.Name = "normalToolStripMenuItem";
            this.normalToolStripMenuItem.Size = System.Drawing.Size(131, 22);
            this.normalToolStripMenuItem.Text = "Normal";
            this.normalToolStripMenuItem.Click += System.EventHandler(this.normalToolStripMenuItem_Click);
            //
            // dPlayPause
            //
            this.dPlayPause.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.dPlayPause.DropDownItems.AddRange(System.Windows.Forms.ToolStripItem[]{
                this.beginToolStripMenuItem,
                this.endToolStripMenuItem,
                this.playPauseToolStripMenuItem});
            this.dPlayPause.Image = ((System.Drawing.Image)(resources.GetObject("dPlayPause.Image")));
            this.dPlayPause.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.dPlayPause.Name = "dPlayPause";
            this.dPlayPause.Size = System.Drawing.Size(32, 22);
            this.dPlayPause.Text = "toolStripSplitButton2";
            this.dPlayPause.ButtonClick += System.EventHandler(this.dPlayPause_ButtonClick);
            //
            // beginToolStripMenuItem
            //
            this.beginToolStripMenuItem.Name = "beginToolStripMenuItem";
            this.beginToolStripMenuItem.Size = System.Drawing.Size(152, 22);
            this.beginToolStripMenuItem.Text = "Begin";
            this.beginToolStripMenuItem.Click += System.EventHandler(this.beginToolStripMenuItem_Click);
            //
            // endToolStripMenuItem
            //
            this.endToolStripMenuItem.Name = "endToolStripMenuItem";
            this.endToolStripMenuItem.Size = System.Drawing.Size(152, 22);
            this.endToolStripMenuItem.Text = "End";
            this.endToolStripMenuItem.Click += System.EventHandler(this.endToolStripMenuItem_Click);
            //
            // playPauseToolStripMenuItem
            //
            this.playPauseToolStripMenuItem.Name = "playPauseToolStripMenuItem";
            this.playPauseToolStripMenuItem.Size = System.Drawing.Size(152, 22);
            this.playPauseToolStripMenuItem.Text = "Play /pause";
            this.playPauseToolStripMenuItem.Click += System.EventHandler(this.playPauseToolStripMenuItem_Click);
            //
            // dAnimations
            //
            this.dAnimations.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.dAnimations.Image = ((System.Drawing.Image)(resources.GetObject("dAnimations.Image")));
            this.dAnimations.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.dAnimations.Name = "dAnimations";
            this.dAnimations.Size = System.Drawing.Size(105, 22);
            this.dAnimations.Text = "No animations";
            //
            // dMeshInfo
            //
            this.dMeshInfo.Name = "dMeshInfo";
            this.dMeshInfo.Size = System.Drawing.Size(0, 19);
            //
            // dAnimInfo
            //
            this.dAnimInfo.Name = "dAnimInfo";
            this.dAnimInfo.Size = System.Drawing.Size(0, 19);
            //
            // dImportDialog
            //
            this.dImportDialog.Filter = "Actor files|*.act|Mesh Def Files|*.mdf|Wavefront obj files|*.obj";
            this.dImportDialog.Title = "Select file to import";
            //
            // dAnimation
            //
            this.dAnimation.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) | System.Windows.Forms.AnchorStyles.Right)));
            this.dAnimation.LargeChange = 50;
            this.dAnimation.Location = System.Drawing.Point(0, 417);
            this.dAnimation.Maximum = 1000;
            this.dAnimation.Name = "dAnimation";
            this.dAnimation.Size = System.Drawing.Size(732, 45);
            this.dAnimation.SmallChange = 10;
            this.dAnimation.TabIndex = 4;
            this.dAnimation.TickStyle = System.Windows.Forms.TickStyle.None;
            this.dAnimation.ValueChanged += System.EventHandler(this.dAnimation_ValueChanged);
            //
            // dSelectAnimation
            //
            this.dSelectAnimation.Filter = "Animations|*anm";
            //
            // dAnimTimer
            //
            this.dAnimTimer.Interval = 41;
            this.dAnimTimer.Tick += System.EventHandler(this.dAnimTimer_Tick);
            //
            // dView
            //
            this.dView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) | System.Windows.Forms.AnchorStyles.Left) | System.Windows.Forms.AnchorStyles.Right)));
            this.dView.Location = System.Drawing.Point(0, 2);
            this.dView.Name = "dView";
            this.dView.Size = System.Drawing.Size(732, 409);
            this.dView.TabIndex = 3;
            //
            // Main
            //
            this.AutoScaleDimensions = System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = System.Drawing.Size(732, 474);
            this.Controls.Add(this.dView);
            this.Controls.Add(this.dStrip);
            this.Controls.Add(this.dAnimation);
            this.Name = "Main";
            this.Text = "Model View";
            this.FormClosed += System.Windows.Forms.FormClosedEventHandler(this.Main_FormClosed);
            this.dStrip.ResumeLayout(false);
            this.dStrip.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dAnimation)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();
        }

#endregion

        System.Windows.Forms.StatusStrip dStrip;
        System.Windows.Forms.OpenFileDialog dImportDialog;
        GlDraw dView;
        System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        System.Windows.Forms.ToolStripMenuItem arcBallToolStripMenuItem;
        System.Windows.Forms.ToolStripMenuItem normalToolStripMenuItem;
        System.Windows.Forms.TrackBar dAnimation;
        System.Windows.Forms.OpenFileDialog dSelectAnimation;
        System.Windows.Forms.ToolStripSplitButton dModelActions;
        System.Windows.Forms.ToolStripMenuItem importToolStripMenuItem1;
        System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem;
        System.Windows.Forms.ToolStripDropDownButton dAnimations;
        System.Windows.Forms.ToolStripSplitButton dPlayPause;
        System.Windows.Forms.ToolStripMenuItem playPauseToolStripMenuItem;
        System.Windows.Forms.ToolStripMenuItem beginToolStripMenuItem;
        System.Windows.Forms.ToolStripMenuItem endToolStripMenuItem;
        System.Windows.Forms.ToolStripStatusLabel dMeshInfo;
        System.Windows.Forms.ToolStripStatusLabel dAnimInfo;
        System.Windows.Forms.ToolStripMenuItem selectMaterialToolStripMenuItem;
        System.Windows.Forms.Timer dAnimTimer;
    }
}
