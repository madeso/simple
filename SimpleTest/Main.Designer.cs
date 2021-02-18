namespace SimpleTest
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
            this.dgl = new Tao.Platform.Windows.SimpleOpenGlControl();
            this.SuspendLayout();
            // 
            // dgl
            // 
            this.dgl.AccumBits = ((byte)(0));
            this.dgl.AutoCheckErrors = false;
            this.dgl.AutoFinish = false;
            this.dgl.AutoMakeCurrent = true;
            this.dgl.AutoSwapBuffers = true;
            this.dgl.BackColor = System.Drawing.Color.Black;
            this.dgl.ColorBits = ((byte)(32));
            this.dgl.DepthBits = ((byte)(16));
            this.dgl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgl.Location = new System.Drawing.Point(0, 0);
            this.dgl.Name = "dgl";
            this.dgl.Size = new System.Drawing.Size(292, 265);
            this.dgl.StencilBits = ((byte)(0));
            this.dgl.TabIndex = 0;
            this.dgl.Paint += new System.Windows.Forms.PaintEventHandler(this.dgl_Paint);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 265);
            this.Controls.Add(this.dgl);
            this.Name = "Main";
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Main_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private Tao.Platform.Windows.SimpleOpenGlControl dgl;
    }
}

