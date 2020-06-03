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
        private directZobCLI.CameraManagerWrapper m_camerManagerWrapper;
        private Form1 m_mainForm = null;
        public CameraControl(Form1 f)
        {
            InitializeComponent();
            m_mainForm = f;
            m_camerManagerWrapper = new directZobCLI.CameraManagerWrapper();
            m_mainForm.OnSceneUpdated += new EventHandler(OnSceneUpdated);
        }

        public directZobCLI.CameraManagerWrapper GetWrapper()
        {
            return m_camerManagerWrapper;
        }

        public void UpdateControl()
        {
            string s = m_camerManagerWrapper.GetCurrentCamera();
            string[] c = m_camerManagerWrapper.GetCameraList();
            if(cameraBox.Items.Count != c.Count())
            {
                cameraBox.Items.Clear();
                for (int i = 0; i < c.Count(); i++)
                {
                    cameraBox.Items.Add(c[i]);
                    if(c[i] == s)
                    {
                        cameraBox.SelectedIndex = i;
                    }
                }
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
            string cn = m_camerManagerWrapper.GetCurrentCamera();
            string [] c = m_camerManagerWrapper.GetCameraList();
            cameraBox.Items.Clear();
            for (int i=0; i<c.Count(); i++)
            {
                cameraBox.Items.Add(c[i]);
                if (c[i] == cn)
                {
                    cameraBox.SelectedIndex = i;
                }
            }
        }
    }
}
