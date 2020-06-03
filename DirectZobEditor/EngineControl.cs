﻿using System;
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
        private directZobCLI.EngineWrapper m_engineWrapper;
        private Form1 m_mainForm = null;
        public EngineControl(Form1 f, directZobCLI.EngineWrapper e)
        {
            m_mainForm = f;
            InitializeComponent();
            m_engineWrapper = e;
            showGrid.Checked = true;
            wireframe.Checked = false;
            showNormals.Checked = false;
            comboBuffer.SelectedIndex = 0;
            comboRender.SelectedIndex = 0;
            comboLighting.SelectedIndex = 1;
            m_engineWrapper.ShowGrid(showGrid.Checked);
            m_engineWrapper.WireFrame(wireframe.Checked);
            m_engineWrapper.DrawGizmos(drawGizmos.Checked);
            engineWidth.Text = m_engineWrapper.GetBufferWidth().ToString();
            engineHeight.Text = m_engineWrapper.GetBufferHeight().ToString();
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
            string sw = engineWidth.Text;
            string sh = engineHeight.Text;
            int w = 0;
            int h = 0;
            if(int.TryParse(sw, out w) && int.TryParse(sh, out h))
            {
                m_mainForm.GetDirectZobWrapper().Resize(w, h);
                Form1.GetMainForm().GetEngineWindow().ResizeRenderWindow();
            }
        }

        private void showNormals_CheckedChanged(object sender, EventArgs e)
        {
            m_engineWrapper.ShowNormals(showNormals.Checked);
        }
        private void ComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox c = (ComboBox)sender;
            m_engineWrapper.SetRenderOutput(c.SelectedIndex);
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
    }
}
