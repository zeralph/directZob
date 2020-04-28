namespace DirectZobEditor
{
    partial class EngineControl
    {
        /// <summary> 
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur de composants

        /// <summary> 
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas 
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.engineSetSize = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.engineHeight = new System.Windows.Forms.TextBox();
            this.engineWidth = new System.Windows.Forms.TextBox();
            this.comboRendering = new System.Windows.Forms.ComboBox();
            this.wireframe = new System.Windows.Forms.CheckBox();
            this.showGrid = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.showNormals = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.showNormals);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.engineSetSize);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.engineHeight);
            this.groupBox1.Controls.Add(this.engineWidth);
            this.groupBox1.Controls.Add(this.comboRendering);
            this.groupBox1.Controls.Add(this.wireframe);
            this.groupBox1.Controls.Add(this.showGrid);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(199, 194);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Engine";
            // 
            // engineSetSize
            // 
            this.engineSetSize.Location = new System.Drawing.Point(144, 103);
            this.engineSetSize.Name = "engineSetSize";
            this.engineSetSize.Size = new System.Drawing.Size(40, 23);
            this.engineSetSize.TabIndex = 7;
            this.engineSetSize.Text = "Set";
            this.engineSetSize.UseVisualStyleBackColor = true;
            this.engineSetSize.Click += new System.EventHandler(this.engineSetSize_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(74, 108);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(15, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "H";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(5, 108);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(18, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "W";
            // 
            // engineHeight
            // 
            this.engineHeight.Location = new System.Drawing.Point(91, 104);
            this.engineHeight.Name = "engineHeight";
            this.engineHeight.Size = new System.Drawing.Size(46, 20);
            this.engineHeight.TabIndex = 4;
            // 
            // engineWidth
            // 
            this.engineWidth.Location = new System.Drawing.Point(23, 104);
            this.engineWidth.Name = "engineWidth";
            this.engineWidth.Size = new System.Drawing.Size(46, 20);
            this.engineWidth.TabIndex = 3;
            // 
            // comboRendering
            // 
            this.comboRendering.FormattingEnabled = true;
            this.comboRendering.Items.AddRange(new object[] {
            "Render",
            "ZBuffer",
            "OBuffer"});
            this.comboRendering.Location = new System.Drawing.Point(67, 76);
            this.comboRendering.Name = "comboRendering";
            this.comboRendering.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.comboRendering.Size = new System.Drawing.Size(117, 21);
            this.comboRendering.TabIndex = 2;
            this.comboRendering.SelectedIndexChanged += new System.EventHandler(this.ComboBox1_SelectedIndexChanged);
            // 
            // wireframe
            // 
            this.wireframe.AutoSize = true;
            this.wireframe.Location = new System.Drawing.Point(110, 19);
            this.wireframe.Name = "wireframe";
            this.wireframe.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.wireframe.Size = new System.Drawing.Size(74, 17);
            this.wireframe.TabIndex = 1;
            this.wireframe.Text = "Wireframe";
            this.wireframe.UseVisualStyleBackColor = true;
            this.wireframe.CheckedChanged += new System.EventHandler(this.Wireframe_CheckedChanged);
            // 
            // showGrid
            // 
            this.showGrid.AutoSize = true;
            this.showGrid.Location = new System.Drawing.Point(111, 53);
            this.showGrid.Name = "showGrid";
            this.showGrid.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.showGrid.Size = new System.Drawing.Size(73, 17);
            this.showGrid.TabIndex = 0;
            this.showGrid.Text = "Show grid";
            this.showGrid.UseVisualStyleBackColor = true;
            this.showGrid.CheckedChanged += new System.EventHandler(this.ShowGrid_CheckedChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(20, 80);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Buffer";
            // 
            // showNormals
            // 
            this.showNormals.AutoSize = true;
            this.showNormals.Location = new System.Drawing.Point(92, 36);
            this.showNormals.Name = "showNormals";
            this.showNormals.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.showNormals.Size = new System.Drawing.Size(92, 17);
            this.showNormals.TabIndex = 9;
            this.showNormals.Text = "Show normals";
            this.showNormals.UseVisualStyleBackColor = true;
            this.showNormals.CheckedChanged += new System.EventHandler(this.showNormals_CheckedChanged);
            // 
            // EngineControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "EngineControl";
            this.Size = new System.Drawing.Size(199, 194);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox wireframe;
        private System.Windows.Forms.CheckBox showGrid;
        private System.Windows.Forms.ComboBox comboRendering;
        private System.Windows.Forms.Button engineSetSize;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox engineHeight;
        private System.Windows.Forms.TextBox engineWidth;
        private System.Windows.Forms.CheckBox showNormals;
        private System.Windows.Forms.Label label3;
    }
}
