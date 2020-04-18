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
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.wireframe = new System.Windows.Forms.CheckBox();
            this.showGrid = new System.Windows.Forms.CheckBox();
            this.engineWidth = new System.Windows.Forms.TextBox();
            this.engineHeight = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.engineSetSize = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.engineSetSize);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.engineHeight);
            this.groupBox1.Controls.Add(this.engineWidth);
            this.groupBox1.Controls.Add(this.comboBox1);
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
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "Render",
            "ZBuffer",
            "OBuffer"});
            this.comboBox1.Location = new System.Drawing.Point(62, 83);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 21);
            this.comboBox1.TabIndex = 2;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.ComboBox1_SelectedIndexChanged);
            // 
            // wireframe
            // 
            this.wireframe.AutoSize = true;
            this.wireframe.Location = new System.Drawing.Point(62, 59);
            this.wireframe.Name = "wireframe";
            this.wireframe.Size = new System.Drawing.Size(74, 17);
            this.wireframe.TabIndex = 1;
            this.wireframe.Text = "Wireframe";
            this.wireframe.UseVisualStyleBackColor = true;
            this.wireframe.CheckedChanged += new System.EventHandler(this.Wireframe_CheckedChanged);
            // 
            // showGrid
            // 
            this.showGrid.AutoSize = true;
            this.showGrid.Location = new System.Drawing.Point(62, 35);
            this.showGrid.Name = "showGrid";
            this.showGrid.Size = new System.Drawing.Size(73, 17);
            this.showGrid.TabIndex = 0;
            this.showGrid.Text = "Show grid";
            this.showGrid.UseVisualStyleBackColor = true;
            this.showGrid.CheckedChanged += new System.EventHandler(this.ShowGrid_CheckedChanged);
            // 
            // engineWidth
            // 
            this.engineWidth.Location = new System.Drawing.Point(22, 115);
            this.engineWidth.Name = "engineWidth";
            this.engineWidth.Size = new System.Drawing.Size(46, 20);
            this.engineWidth.TabIndex = 3;
            // 
            // engineHeight
            // 
            this.engineHeight.Location = new System.Drawing.Point(90, 115);
            this.engineHeight.Name = "engineHeight";
            this.engineHeight.Size = new System.Drawing.Size(46, 20);
            this.engineHeight.TabIndex = 4;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 122);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(18, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "W";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(73, 122);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(15, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "H";
            // 
            // engineSetSize
            // 
            this.engineSetSize.Location = new System.Drawing.Point(143, 113);
            this.engineSetSize.Name = "engineSetSize";
            this.engineSetSize.Size = new System.Drawing.Size(40, 23);
            this.engineSetSize.TabIndex = 7;
            this.engineSetSize.Text = "Set";
            this.engineSetSize.UseVisualStyleBackColor = true;
            this.engineSetSize.Click += new System.EventHandler(this.engineSetSize_Click);
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
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button engineSetSize;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox engineHeight;
        private System.Windows.Forms.TextBox engineWidth;
    }
}
