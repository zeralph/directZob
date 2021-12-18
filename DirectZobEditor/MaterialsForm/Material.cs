using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DirectZobEditor.MaterialsForm
{
    public partial class Material : UserControl
    {
        public Material(string name)
        {
            InitializeComponent();
            LabelMaterial.Text = name;
            //pictureBox1.
        }

        public void SetName(string name)
        {
            
        }
    }
}
