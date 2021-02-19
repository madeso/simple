namespace ModelView
{
    struct GlDraw
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System.ComponentModel.IContainer components = nullptr;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>

        override void Dispose(bool disposing)
        {
            if (disposing && (components != nullptr))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

#region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent()
        {
            this.SuspendLayout();
            //
            // GlDraw
            //
            this.AutoScaleDimensions = System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "GlDraw";
            this.Size = System.Drawing.Size(300, 271);
            this.ResumeLayout(false);
        }

#endregion
    }
}
