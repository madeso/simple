using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SimpleEngine;

namespace ModelView
{
    public partial class ChangeMaterial : Form
    {
        private string texture = null;

        public string Texture
        {
            get
            {
                return texture;
            }
        }

        public string MaterialName
        {
            get
            {
                MeshDef.MaterialDef mat = (MeshDef.MaterialDef)dMaterials.SelectedItem;
                return mat.name;
            }
        }

        public ChangeMaterial(MeshDef def)
        {
            InitializeComponent();

            foreach (MeshDef.MaterialDef m in def.Materials)
            {
                dMaterials.Items.Add(m);
            }
            dMaterials.Text = dMaterials.Items[0].ToString();
        }

        private void dOk_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(texture))
            {
                return;
            }
            else
            {
                DialogResult = DialogResult.OK;
                Close();
            }
        }

        private void dAbort_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Abort;
            Close();
        }

        private void dSelectTexture_Click(object sender, EventArgs e)
        {
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                texture = ofd.FileName;
                dSelectedTexture.Text = texture;
            }
        }
    }
}
