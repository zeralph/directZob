using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DirectZobEditor.MaterialsForm
{
    public partial class MaterialsForm : Form
    {
        private CLI.DirectZobWrapper m_directZobWrapper;

        public MaterialsForm(CLI.DirectZobWrapper directZobWrapper)
        {
            InitializeComponent();
            m_directZobWrapper = directZobWrapper;
            m_directZobWrapper.GetEngineWrapper();
        }
    }
}
