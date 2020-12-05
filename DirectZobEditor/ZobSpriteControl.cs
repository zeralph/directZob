using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CLI;

namespace DirectZobEditor
{
    public partial class ZobSpriteControl : UserControl
    {
        private Form1 m_mainForm = null;
        private ZobSpriteWrapper m_sprite = null;
        public ZobSpriteControl(Form1 f)
        {
            m_mainForm = f;
            InitializeComponent();
            this.Visible = false;
        }
        public void BindEvents()
        {
            m_mainForm.GetZobObjectListControl().OnObjectSelected += new ZobObjectListControl.OnObjectSelectedHandler(OnObjectSelected);
        }
        private void OnObjectSelected(object s, ObjectSelectionEventArg e)
        {
            if (e.newZobObject != null)
            {
                m_sprite = e.newZobObject as ZobSpriteWrapper;
                if (m_sprite != null)
                {
                    this.Visible = true;
                }
                else
                {
                    this.Visible = false;
                }
            }
            else
            {
                this.Visible = false;
            }
            //UpdateValues();
            //RefreshInterface();
        }

        private void buttonTexture_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = m_mainForm.Getpath();
                openFileDialog.Filter = "all images (*.tga*.png*.jpg*.jpeg)|*.png;*.tga;*.jpg;*.jpeg";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;
                //openFileDialog.AutoUpgradeEnabled = false;
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    string path = openFileDialog.FileName;
                    textBoxTexturePath.Text = path;
                    if(m_sprite != null)
                    {
                        m_sprite.SetTexture(path);
                    }
                }
            }
        }
    }
}
