using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using CLI;
using System.Runtime.CompilerServices;
using System.Numerics;
using System.IO;

namespace DirectZobEditor
{
    public partial class EngineWindow : UserControl
    {
        public EngineWindow(Form1 f)
        {
            InitializeComponent();
            this.Dock = DockStyle.Fill;
            this.DoubleBuffered = true;
        }

        public PictureBox GetEngineRenderwindow()
        {
            return EngineRender;
        }
    }
}
