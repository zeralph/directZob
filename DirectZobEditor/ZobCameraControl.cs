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
    public partial class ZobCameraControl : UserControl
    {
        Form1 m_mainForm = null;
        ZobCameraWrapper m_camera = null;
        public ZobCameraControl(Form1 f)
        {
            InitializeComponent();
            m_mainForm = f;
        }
        public void BindEvents()
        {
            m_mainForm.GetZobObjectListControl().OnObjectSelected += new ZobObjectListControl.OnObjectSelectedHandler(OnObjectSelected);
        }
        private void OnObjectSelected(object s, ObjectSelectionEventArg e)
        {
            if (e.newZobObject != null)
            {
                m_camera = e.newZobObject as ZobCameraWrapper;
                if (m_camera != null)
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
            UpdateValues();
            RefreshInterface();
        }

        private void UpdateValues()
        {
            if(m_camera != null)
            {
                FovValue.Text = m_camera.GetFOV().ToString();

            }
        }
        private void valueChanged(object sender, EventArgs e)
        {
            SetValues();
        }
        private void SetValues()
        {
            if (m_camera != null)
            {
                float f;
                FovValue.Text = FovValue.Text.Replace('.', ',');
                if (float.TryParse(FovValue.Text, out f))
                {
                    m_camera.SetFOV(f);
                }
            }
        }

        private void RefreshInterface()
        {

        }
    }
}
