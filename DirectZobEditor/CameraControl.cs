using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DirectZobEditor
{
    public partial class CameraControl : UserControl
    {
        private CLI.CameraManagerWrapper m_camerManagerWrapper;
        private Form1 m_mainForm = null;
        public CameraControl(Form1 f)
        {
            InitializeComponent();
            m_mainForm = f;
            m_camerManagerWrapper = new CLI.CameraManagerWrapper();
            m_mainForm.OnSceneUpdated += new EventHandler(OnSceneUpdated);
        }

        public CLI.CameraManagerWrapper GetWrapper()
        {
            return m_camerManagerWrapper;
        }

        public void UpdateControl()
        {
            string s = m_camerManagerWrapper.GetCurrentCamera();
            CLI.ManagedVector3 p = m_camerManagerWrapper.GetCurrentCameraPosition();
            CLI.ManagedVector3 t = m_camerManagerWrapper.GetCurrentCameraTarget();
            if(!cameraPosX.Focused)
            {
                cameraPosX.Text = String.Format("{0:0.00}", p.x);
            }
            if (!cameraPosY.Focused)
            {
                cameraPosY.Text = String.Format("{0:0.00}", p.y);
            }
            if (!cameraPosZ.Focused)
            {
                cameraPosZ.Text = String.Format("{0:0.00}", p.z);
            }
            if (!cameraTarX.Focused)
            {
                cameraTarX.Text = String.Format("{0:0.00}", t.x);
            }
            if (!cameraTarY.Focused)
            {
                cameraTarY.Text = String.Format("{0:0.00}", t.y);
            }
            if (!cameraTarZ.Focused)
            {
                cameraTarZ.Text = String.Format("{0:0.00}", t.z);
            }
        }

        private void CameraPosX_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                float f = float.Parse(cameraPosX.Text);
                CLI.ManagedVector3 p = m_camerManagerWrapper.GetCurrentCameraPosition();
                p.x = f;
                m_camerManagerWrapper.SetCurrentCameraPosition(p);
            }
        }

        private void CameraPosY_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                float f = float.Parse(cameraPosY.Text);
                CLI.ManagedVector3 p = m_camerManagerWrapper.GetCurrentCameraPosition();
                p.y = f;
                m_camerManagerWrapper.SetCurrentCameraPosition(p);
            }
        }

        private void CameraPosZ_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                float f = float.Parse(cameraPosZ.Text);
                CLI.ManagedVector3 p = m_camerManagerWrapper.GetCurrentCameraPosition();
                p.z = f;
                m_camerManagerWrapper.SetCurrentCameraPosition(p);
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            string s = (string)cameraBox.SelectedItem;
            if (!string.IsNullOrEmpty(s))
            {
                m_camerManagerWrapper.SetCurrentCamera(s);
            }
        }

        private void OnSceneUpdated(object s, EventArgs e)
        {
            string [] c = m_camerManagerWrapper.GetCameraList();
            cameraBox.Items.Clear();
            for (int i=0; i<c.Count(); i++)
            {
                cameraBox.Items.Add(c[i]);
            }
        }
    }
}
