using DirectZobEditor.Packer;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DirectZobEditor
{
    public partial class ZobPackerForm : Form
    {
        private DirectZobEditorForm m_mainForm;
        private CLI.DirectZobWrapper m_directZobWrapper;
        private ZobPacker m_zobPacker;
        public ZobPackerForm(DirectZobEditorForm f, CLI.DirectZobWrapper directZobWrapper)
        {
            m_directZobWrapper = directZobWrapper;
            m_mainForm = f;
            InitializeComponent();
        }

        private void buttonAddLevel_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                //openFileDialog.InitialDirectory = m_path;
                openFileDialog.Filter = "zob scene (*.dzs)|*.dzs";
                openFileDialog.FilterIndex = 2;
                string workspace = m_directZobWrapper.GetResourcePath();
                if (!String.IsNullOrEmpty(workspace) && Directory.Exists(workspace))
                {
                    openFileDialog.InitialDirectory = workspace.Replace("/", "\\"); ;
                }
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    packLevels.Items.Add(openFileDialog.FileName);
                }
            }
        }

        private void buttonPack_Click(object sender, EventArgs e)
        {
            m_zobPacker = new ZobPacker(m_mainForm, "D:\\");
            for (int i=0; i< packLevels.Items.Count; i++)
            {
                string fullPath= packLevels.Items[i].Text;
                string path = Path.GetDirectoryName(fullPath);
                string file = Path.GetFileName(fullPath);
                m_zobPacker.AddScene(path, file);
            }
            m_zobPacker.FinishPacking(Path.GetFileName(packLevels.Items[0].Text));
        }

        private void Pack(string path, string file)
        {
        }
    }
}
