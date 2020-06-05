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
    public partial class OptionsForm : Form
    {
        public OptionsForm(Form1 f)
        {
            InitializeComponent();
        }

        public void Show(string title, UserControl c)
        {
            this.Text = title;
            this.Size = new Size(1, 1);
            this.AutoSize = true;
            this.Controls.Clear();
            this.Controls.Add(c);
            this.Show();
            this.BringToFront();
            this.TopMost = true;
        }

        private void OptionsForm_Load(object sender, EventArgs e)
        {

        }

        private void OptionsForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            Hide();
            this.Controls.Clear();
        }
    }
}
