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
        public CameraControl()
        {
            InitializeComponent();
            m_camerManagerWrapper = new CLI.CameraManagerWrapper();
        }

        public void Update()
        {
            string s = m_camerManagerWrapper.GetCurrentCamera();
            CLI.ManagedVector3 p = m_camerManagerWrapper.GetCurrentCameraPosition();
            CLI.ManagedVector3 t = m_camerManagerWrapper.GetCurrentCameraTarget();
            //cameraPosX.Text = String.Format("{0:0.00}", p.x);
            //cameraPosY.Text = String.Format("{0:0.00}", p.y);
            //cameraPosZ.Text = String.Format("{0:0.00}", p.z);
            //cameraTarX.Text = String.Format("{0:0.00}", t.x);
            //cameraTarY.Text = String.Format("{0:0.00}", t.y);
            //cameraTarZ.Text = String.Format("{0:0.00}", t.z);
        }
    }
}
