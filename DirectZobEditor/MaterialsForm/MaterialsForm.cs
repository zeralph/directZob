﻿using System;
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
            CLI.ZobMaterialsManagerWrapper manager = new CLI.ZobMaterialsManagerWrapper();
            manager.ParseMaterials();
            int nbMat = manager.GetNbMaterials();
            for (int i = 0; i < nbMat; i++)
            {
                CLI.ZobMaterialWrapper mat = manager.GetMaterial(i);
                Material ctrl = new Material(mat);
                MaterialsTableLayout.Controls.Add(ctrl);
                MaterialsTableLayout.Controls[i].Height = ctrl.Height + 2;
                ctrl.Width = MaterialsTableLayout.Controls[i].Width;
            }
            
        }

        private void MaterialsTableLayout_Resize(object sender, EventArgs e)
        {
            int w = MaterialsTableLayout.Width;
            //MaterialsTableLayout.ColumnCount = (int)(w / 300);
        }
    }
}