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
            m_engineWrapper.ShowGrid(showGrid.Checked);
            m_engineWrapper.WireFrame(wireframe.Checked);
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
    }
}
