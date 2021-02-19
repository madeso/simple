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
    struct ChangeMaterial : Form
    {
        std::string texture = nullptr;

        std::string Texture
        {
            get
            {
                return texture;
            }
        }

        std::string MaterialName
        {
            get
            {
                MeshDef.MaterialDef mat = (MeshDef.MaterialDef)dMaterials.SelectedItem;
                return mat.name;
            }
        }

        ChangeMaterial(MeshDef def)
        {
            InitializeComponent();

            for (MeshDef.MaterialDef m : def.Materials)
            {
                dMaterials.Items.Add(m);
            }
            dMaterials.Text = dMaterials.Items[0].ToString();
        }

        void dOk_Click(object sender, EventArgs e)
        {
            if (std::string.IsNullOrEmpty(texture))
            {
                return;
            }
            else
            {
                DialogResult = DialogResult.OK;
                Close();
            }
        }

        void dAbort_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Abort;
            Close();
        }

        void dSelectTexture_Click(object sender, EventArgs e)
        {
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                texture = ofd.FileName;
                dSelectedTexture.Text = texture;
            }
        }
    }
}
