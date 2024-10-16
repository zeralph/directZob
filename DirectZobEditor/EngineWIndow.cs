﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using System.Runtime.CompilerServices;
using System.Numerics;
using System.IO;
using DirectZobInterface;


namespace DirectZobEditor
{
    public partial class EngineWindow : UserControl
    {
        public EngineWindow(DirectZobEditorForm f)
        {
            InitializeComponent();
            this.Dock = DockStyle.Fill;
            this.DoubleBuffered = true;
            this.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            this.AutoSize = true;
        }

        public PictureBox GetEngineRenderwindow()
        {
            return EngineRender;
        }
    }
}
