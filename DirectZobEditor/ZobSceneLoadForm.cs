using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DirectZobEditor
{
    public partial class ZobSceneLoadForm : Form
    {

        private int m_nbObjects = 0;
        private int m_curObject = 0;


        public ZobSceneLoadForm(DirectZobEditorForm f, CLI.DirectZobWrapper directZobWrapper)
        {
            this.TopMost = true;
            m_nbObjects = 0;
            m_curObject = 0;
            InitializeComponent();
        }

        public void SetText(int nbObj, int curObj, string s)
        {
            if(nbObj != 0)
            {
                m_nbObjects = nbObj;
                m_curObject = 0;
                progressBar1.Value = 0;
                progressBar1.Maximum = m_nbObjects;
            }
            if(m_curObject > progressBar1.Maximum)
            {
                int y = 0;
                y++;
            }
            if (!string.IsNullOrEmpty(s))
            {
                progressBar1.Value = Math.Min(m_curObject, progressBar1.Maximum);
                m_curObject++;
                this.Invoke(new Action(() =>
                {
                    loadingInfo.Text = "" + m_curObject + "/" + m_nbObjects + " " + s;
                }));
            }
        }

        public void AskForClose()
        {
            this.Invoke(new Action(() =>
            {
                this.Close();
            }));
        }
    }
}
