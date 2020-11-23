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
        Form1 m_mainForm;
        public OptionsForm(Form1 f, string title, UserControl c)
        {
            m_mainForm = f;
            this.Text = title;
            this.Size = new Size(1, 1);
            this.AutoSize = true;
            this.Controls.Clear();
            this.Controls.Add(c);  
            this.BringToFront();
            this.TopMost = true;
            Point p = new Point();
            p.X = m_mainForm.Location.X / 2 + m_mainForm.Width / 2 - this.Width / 2;
            p.Y = m_mainForm.Location.Y / 2 + m_mainForm.Height / 2 - this.Height / 2;
            this.Location = p;
            InitializeComponent();
            this.Show();
        }

        private void OptionsForm_Load(object sender, EventArgs e)
        {

        }

        private void OptionsForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            //e.Cancel;
            //Invoke(delegate
            //{
             //   Hide();
             //   this.Controls.Clear();
            //});
        }
    }
}
