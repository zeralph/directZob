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
        private DirectZobInterface.ZobMaterialWrapper m_material;
        public Material(DirectZobInterface.ZobMaterialWrapper mat)
        {
            m_material = mat;
            InitializeComponent();
            LabelMaterial.Text = m_material.m_name + "("+ m_material.m_width +"x"+ m_material.m_height +")";
            SetTexture();
        }

        public void SetName(string name)
        {
            
        }

        private void SetTexture()
        {
            pictureBox1.Image = m_material.GetTexture();
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
        }
    }
}
