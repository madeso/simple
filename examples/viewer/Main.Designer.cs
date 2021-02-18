namespace ModelView
{
    partial class Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
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
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.dStrip = new System.Windows.Forms.StatusStrip();
            this.dModelActions = new System.Windows.Forms.ToolStripSplitButton();
            this.importToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.exportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selectMaterialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.arcBallToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.normalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dPlayPause = new System.Windows.Forms.ToolStripSplitButton();
            this.beginToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.endToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.playPauseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dAnimations = new System.Windows.Forms.ToolStripDropDownButton();
            this.dMeshInfo = new System.Windows.Forms.ToolStripStatusLabel();
            this.dAnimInfo = new System.Windows.Forms.ToolStripStatusLabel();
            this.dImportDialog = new System.Windows.Forms.OpenFileDialog();
            this.dAnimation = new System.Windows.Forms.TrackBar();
            this.dSelectAnimation = new System.Windows.Forms.OpenFileDialog();
            this.dAnimTimer = new System.Windows.Forms.Timer(this.components);
            this.dView = new ModelView.GlDraw();
            this.dStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dAnimation)).BeginInit();
            this.SuspendLayout();
            // 
            // dStrip
            // 
            this.dStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.dModelActions,
            this.toolStripDropDownButton1,
            this.dPlayPause,
            this.dAnimations,
            this.dMeshInfo,
            this.dAnimInfo});
            this.dStrip.Location = new System.Drawing.Point(0, 450);
            this.dStrip.Name = "dStrip";
            this.dStrip.Size = new System.Drawing.Size(732, 24);
            this.dStrip.TabIndex = 2;
            this.dStrip.Text = "statusStrip1";
            // 
            // dModelActions
            // 
            this.dModelActions.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.dModelActions.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.importToolStripMenuItem1,
            this.exportToolStripMenuItem,
            this.selectMaterialToolStripMenuItem});
            this.dModelActions.Image = ((System.Drawing.Image)(resources.GetObject("dModelActions.Image")));
            this.dModelActions.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.dModelActions.Name = "dModelActions";
            this.dModelActions.Size = new System.Drawing.Size(92, 22);
            this.dModelActions.Text = "Load model";
            this.dModelActions.ButtonClick += new System.EventHandler(this.dModelActions_ButtonClick);
            // 
            // importToolStripMenuItem1
            // 
            this.importToolStripMenuItem1.Name = "importToolStripMenuItem1";
            this.importToolStripMenuItem1.Size = new System.Drawing.Size(185, 22);
            this.importToolStripMenuItem1.Text = "Import...";
            this.importToolStripMenuItem1.Click += new System.EventHandler(this.importToolStripMenuItem1_Click);
            // 
            // exportToolStripMenuItem
            // 
            this.exportToolStripMenuItem.Name = "exportToolStripMenuItem";
            this.exportToolStripMenuItem.Size = new System.Drawing.Size(185, 22);
            this.exportToolStripMenuItem.Text = "Export...";
            this.exportToolStripMenuItem.Click += new System.EventHandler(this.exportToolStripMenuItem_Click);
            // 
            // selectMaterialToolStripMenuItem
            // 
            this.selectMaterialToolStripMenuItem.Name = "selectMaterialToolStripMenuItem";
            this.selectMaterialToolStripMenuItem.Size = new System.Drawing.Size(185, 22);
            this.selectMaterialToolStripMenuItem.Text = "Select Material...";
            this.selectMaterialToolStripMenuItem.Click += new System.EventHandler(this.selectMaterialToolStripMenuItem_Click);
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripDropDownButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.arcBallToolStripMenuItem,
            this.normalToolStripMenuItem});
            this.toolStripDropDownButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton1.Image")));
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(118, 22);
            this.toolStripDropDownButton1.Text = "Rotation method";
            // 
            // arcBallToolStripMenuItem
            // 
            this.arcBallToolStripMenuItem.Name = "arcBallToolStripMenuItem";
            this.arcBallToolStripMenuItem.Size = new System.Drawing.Size(131, 22);
            this.arcBallToolStripMenuItem.Text = "Arc Ball";
            this.arcBallToolStripMenuItem.Click += new System.EventHandler(this.arcBallToolStripMenuItem_Click);
            // 
            // normalToolStripMenuItem
            // 
            this.normalToolStripMenuItem.Name = "normalToolStripMenuItem";
            this.normalToolStripMenuItem.Size = new System.Drawing.Size(131, 22);
            this.normalToolStripMenuItem.Text = "Normal";
            this.normalToolStripMenuItem.Click += new System.EventHandler(this.normalToolStripMenuItem_Click);
            // 
            // dPlayPause
            // 
            this.dPlayPause.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.dPlayPause.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.beginToolStripMenuItem,
            this.endToolStripMenuItem,
            this.playPauseToolStripMenuItem});
            this.dPlayPause.Image = ((System.Drawing.Image)(resources.GetObject("dPlayPause.Image")));
            this.dPlayPause.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.dPlayPause.Name = "dPlayPause";
            this.dPlayPause.Size = new System.Drawing.Size(32, 22);
            this.dPlayPause.Text = "toolStripSplitButton2";
            this.dPlayPause.ButtonClick += new System.EventHandler(this.dPlayPause_ButtonClick);
            // 
            // beginToolStripMenuItem
            // 
            this.beginToolStripMenuItem.Name = "beginToolStripMenuItem";
            this.beginToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.beginToolStripMenuItem.Text = "Begin";
            this.beginToolStripMenuItem.Click += new System.EventHandler(this.beginToolStripMenuItem_Click);
            // 
            // endToolStripMenuItem
            // 
            this.endToolStripMenuItem.Name = "endToolStripMenuItem";
            this.endToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.endToolStripMenuItem.Text = "End";
            this.endToolStripMenuItem.Click += new System.EventHandler(this.endToolStripMenuItem_Click);
            // 
            // playPauseToolStripMenuItem
            // 
            this.playPauseToolStripMenuItem.Name = "playPauseToolStripMenuItem";
            this.playPauseToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.playPauseToolStripMenuItem.Text = "Play /pause";
            this.playPauseToolStripMenuItem.Click += new System.EventHandler(this.playPauseToolStripMenuItem_Click);
            // 
            // dAnimations
            // 
            this.dAnimations.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.dAnimations.Image = ((System.Drawing.Image)(resources.GetObject("dAnimations.Image")));
            this.dAnimations.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.dAnimations.Name = "dAnimations";
            this.dAnimations.Size = new System.Drawing.Size(105, 22);
            this.dAnimations.Text = "No animations";
            // 
            // dMeshInfo
            // 
            this.dMeshInfo.Name = "dMeshInfo";
            this.dMeshInfo.Size = new System.Drawing.Size(0, 19);
            // 
            // dAnimInfo
            // 
            this.dAnimInfo.Name = "dAnimInfo";
            this.dAnimInfo.Size = new System.Drawing.Size(0, 19);
            // 
            // dImportDialog
            // 
            this.dImportDialog.Filter = "Actor files|*.act|Mesh Def Files|*.mdf|Wavefront obj files|*.obj";
            this.dImportDialog.Title = "Select file to import";
            // 
            // dAnimation
            // 
            this.dAnimation.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dAnimation.LargeChange = 50;
            this.dAnimation.Location = new System.Drawing.Point(0, 417);
            this.dAnimation.Maximum = 1000;
            this.dAnimation.Name = "dAnimation";
            this.dAnimation.Size = new System.Drawing.Size(732, 45);
            this.dAnimation.SmallChange = 10;
            this.dAnimation.TabIndex = 4;
            this.dAnimation.TickStyle = System.Windows.Forms.TickStyle.None;
            this.dAnimation.ValueChanged += new System.EventHandler(this.dAnimation_ValueChanged);
            // 
            // dSelectAnimation
            // 
            this.dSelectAnimation.Filter = "Animations|*anm";
            // 
            // dAnimTimer
            // 
            this.dAnimTimer.Interval = 41;
            this.dAnimTimer.Tick += new System.EventHandler(this.dAnimTimer_Tick);
            // 
            // dView
            // 
            this.dView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dView.Location = new System.Drawing.Point(0, 2);
            this.dView.Name = "dView";
            this.dView.Size = new System.Drawing.Size(732, 409);
            this.dView.TabIndex = 3;
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(732, 474);
            this.Controls.Add(this.dView);
            this.Controls.Add(this.dStrip);
            this.Controls.Add(this.dAnimation);
            this.Name = "Main";
            this.Text = "Model View";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Main_FormClosed);
            this.dStrip.ResumeLayout(false);
            this.dStrip.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dAnimation)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip dStrip;
        private System.Windows.Forms.OpenFileDialog dImportDialog;
        private GlDraw dView;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.ToolStripMenuItem arcBallToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem normalToolStripMenuItem;
        private System.Windows.Forms.TrackBar dAnimation;
        private System.Windows.Forms.OpenFileDialog dSelectAnimation;
        private System.Windows.Forms.ToolStripSplitButton dModelActions;
        private System.Windows.Forms.ToolStripMenuItem importToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem;
        private System.Windows.Forms.ToolStripDropDownButton dAnimations;
        private System.Windows.Forms.ToolStripSplitButton dPlayPause;
        private System.Windows.Forms.ToolStripMenuItem playPauseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem beginToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem endToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel dMeshInfo;
        private System.Windows.Forms.ToolStripStatusLabel dAnimInfo;
        private System.Windows.Forms.ToolStripMenuItem selectMaterialToolStripMenuItem;
        private System.Windows.Forms.Timer dAnimTimer;
    }
}

