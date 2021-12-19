using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DirectZobEditor.MaterialsForm
{
    public partial class Material : UserControl
    {
        private CLI.ZobMaterialWrapper m_material;
        public Material(CLI.ZobMaterialWrapper mat)
        {
            m_material = mat;
            InitializeComponent();
            LabelMaterial.Text = m_material.name;
            SetTexture();
        }

        public void SetName(string name)
        {
            
        }

        private void SetTexture()
        {
            pictureBox1.Image = m_material.texture;
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
        }
    }
}
