namespace ModelView
{
    partial class ChangeMaterial
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
            this.dMaterials = new System.Windows.Forms.ComboBox();
            this.dSelectTexture = new System.Windows.Forms.Button();
            this.dSelectedTexture = new System.Windows.Forms.TextBox();
            this.ofd = new System.Windows.Forms.OpenFileDialog();
            this.dOk = new System.Windows.Forms.Button();
            this.dAbort = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // dMaterials
            // 
            this.dMaterials.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dMaterials.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.dMaterials.FormattingEnabled = true;
            this.dMaterials.Location = new System.Drawing.Point(12, 12);
            this.dMaterials.Name = "dMaterials";
            this.dMaterials.Size = new System.Drawing.Size(268, 21);
            this.dMaterials.TabIndex = 0;
            // 
            // dSelectTexture
            // 
            this.dSelectTexture.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.dSelectTexture.Location = new System.Drawing.Point(254, 37);
            this.dSelectTexture.Name = "dSelectTexture";
            this.dSelectTexture.Size = new System.Drawing.Size(26, 23);
            this.dSelectTexture.TabIndex = 1;
            this.dSelectTexture.Text = "...";
            this.dSelectTexture.UseVisualStyleBackColor = true;
            this.dSelectTexture.Click += new System.EventHandler(this.dSelectTexture_Click);
            // 
            // dSelectedTexture
            // 
            this.dSelectedTexture.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.dSelectedTexture.Location = new System.Drawing.Point(12, 39);
            this.dSelectedTexture.Name = "dSelectedTexture";
            this.dSelectedTexture.ReadOnly = true;
            this.dSelectedTexture.Size = new System.Drawing.Size(236, 20);
            this.dSelectedTexture.TabIndex = 2;
            // 
            // ofd
            // 
            this.ofd.Filter = "Jpeg|*.jpg|Png|*.png|All files|*.*";
            // 
            // dOk
            // 
            this.dOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.dOk.Location = new System.Drawing.Point(205, 70);
            this.dOk.Name = "dOk";
            this.dOk.Size = new System.Drawing.Size(75, 23);
            this.dOk.TabIndex = 3;
            this.dOk.Text = "OK";
            this.dOk.UseVisualStyleBackColor = true;
            this.dOk.Click += new System.EventHandler(this.dOk_Click);
            // 
            // dAbort
            // 
            this.dAbort.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.dAbort.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.dAbort.Location = new System.Drawing.Point(124, 70);
            this.dAbort.Name = "dAbort";
            this.dAbort.Size = new System.Drawing.Size(75, 23);
            this.dAbort.TabIndex = 4;
            this.dAbort.Text = "Abort";
            this.dAbort.UseVisualStyleBackColor = true;
            this.dAbort.Click += new System.EventHandler(this.dAbort_Click);
            // 
            // ChangeMaterial
            // 
            this.AcceptButton = this.dOk;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.dAbort;
            this.ClientSize = new System.Drawing.Size(292, 105);
            this.Controls.Add(this.dAbort);
            this.Controls.Add(this.dOk);
            this.Controls.Add(this.dSelectedTexture);
            this.Controls.Add(this.dSelectTexture);
            this.Controls.Add(this.dMaterials);
            this.Name = "ChangeMaterial";
            this.Text = "ChangeMaterial";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox dMaterials;
        private System.Windows.Forms.Button dSelectTexture;
        private System.Windows.Forms.TextBox dSelectedTexture;
        private System.Windows.Forms.OpenFileDialog ofd;
        private System.Windows.Forms.Button dOk;
        private System.Windows.Forms.Button dAbort;
    }
}