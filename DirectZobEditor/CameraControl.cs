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
        }
        public void BindEvents()
        {
            m_camerManagerWrapper = new CLI.CameraManagerWrapper();
            m_mainForm.OnSceneUpdated += new Form1.OnSceneUpdateHandler(OnSceneUpdated);
        }
        public CLI.CameraManagerWrapper GetWrapper()
        {
            return m_camerManagerWrapper;
        }

        public void UpdateControl()
        {

        }

        
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            string s = (string)cameraBox.SelectedItem;
            if (!string.IsNullOrEmpty(s))
            {
                m_camerManagerWrapper.SetCurrentCamera(s);
            }
        }

        private void OnSceneUpdated(object s, Form1.SceneUpdateEventArg e)
        {
            string cn = m_camerManagerWrapper.GetCurrentCameraName();
            string [] c = m_camerManagerWrapper.GetCameraList();
            cameraBox.Items.Clear();
            for (int i=0; i<c.Count(); i++)
            {
                cameraBox.Items.Add(c[i]);
                if (c[i] == cn)
                {
//                    cameraBox.SelectedIndex = i;
                }
            }
        }
    }
}
