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
        public EngineControl(CLI.EngineWrapper e)
        {
            InitializeComponent();
            m_engineWrapper = e;
            showGrid.Checked = true;
            wireframe.Checked = false;
            showNormals.Checked = false;
            comboRendering.SelectedIndex = 0;
            m_engineWrapper.ShowGrid(showGrid.Checked);
            m_engineWrapper.WireFrame(wireframe.Checked);
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

        private void ComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox c = (ComboBox)sender;
            m_engineWrapper.SetRenderOutput(c.SelectedIndex);
        }

        private void engineSetSize_Click(object sender, EventArgs e)
        {
            string sw = engineWidth.Text;
            string sh = engineHeight.Text;
            int w = 0;
            int h = 0;
            if(int.TryParse(sw, out w) && int.TryParse(sh, out h))
            {
                m_engineWrapper.Resize(w, h);
                Form1.GetMainForm().GetEngineWindow().ResizeRenderWindow();
            }
        }

        private void showNormals_CheckedChanged(object sender, EventArgs e)
        {
            m_engineWrapper.ShowNormals(showNormals.Checked);
        }
    }
}
