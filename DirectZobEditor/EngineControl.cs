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
    public partial class EngineControl : UserControl
    {
        private CLI.EngineWrapper m_engineWrapper;
        private Form1 m_mainForm = null;
        public EngineControl(Form1 f, CLI.EngineWrapper e)
        {
            m_mainForm = f;
            InitializeComponent();
            m_engineWrapper = e;
            showGrid.Checked = true;
            wireframe.Checked = false;
            showNormals.Checked = false;
            zBuffer.Checked = false;
            comboRender.SelectedIndex = 0;
            comboLighting.SelectedIndex = 1;
            m_engineWrapper.ShowGrid(showGrid.Checked);
            m_engineWrapper.WireFrame(wireframe.Checked);
            m_engineWrapper.DrawGizmos(drawGizmos.Checked);
            engineWidth.Text = m_engineWrapper.GetBufferWidth().ToString();
            engineHeight.Text = m_engineWrapper.GetBufferHeight().ToString();
            lockFrustrum.Checked = m_engineWrapper.LockFrustrum();
        }

        public void BindEvents()
        {

        }

        private void ShowGrid_CheckedChanged(object sender, EventArgs e)
        {
            m_engineWrapper.ShowGrid(showGrid.Checked);
        }

        private void Wireframe_CheckedChanged(object sender, EventArgs e)
        {
            m_engineWrapper.WireFrame(wireframe.Checked);
        }

        private void engineSetSize_Click(object sender, EventArgs e)
        {
        }

        private void showNormals_CheckedChanged(object sender, EventArgs e)
        {
            m_engineWrapper.ShowNormals(showNormals.Checked);
        }
        private void ComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox c = (ComboBox)sender;
            //m_engineWrapper.SetRenderOutput(c.SelectedIndex);
        }

        private void comboRender_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox c = (ComboBox)sender;
            m_engineWrapper.SetRenderMode(c.SelectedIndex);
        }

        private void comboLighting_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox c = (ComboBox)sender;
            m_engineWrapper.SetLightingPrecision(c.SelectedIndex);
        }

        private void drawGizmos_CheckedChanged(object sender, EventArgs e)
        {
            m_engineWrapper.DrawGizmos(drawGizmos.Checked);
        }

        private void lockFrustrum_CheckedChanged(object sender, EventArgs e)
        {
            bool bLock = m_engineWrapper.LockFrustrum();
            bLock = !bLock;
            lockFrustrum.Checked = bLock;
            m_engineWrapper.LockFrustrum(bLock);
        }

        private void zBuffer_CheckedChanged(object sender, EventArgs e)
        {
            m_engineWrapper.ToggleZbufferOutput(true);
        }
    }
}
