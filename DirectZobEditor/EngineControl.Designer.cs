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
            this.showNormals = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.engineSetSize = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.engineHeight = new System.Windows.Forms.TextBox();
            this.engineWidth = new System.Windows.Forms.TextBox();
            this.comboBuffer = new System.Windows.Forms.ComboBox();
            this.wireframe = new System.Windows.Forms.CheckBox();
            this.showGrid = new System.Windows.Forms.CheckBox();
            this.drawGizmos = new System.Windows.Forms.CheckBox();
            this.label4 = new System.Windows.Forms.Label();
            this.comboRender = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.comboLighting = new System.Windows.Forms.ComboBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.comboLighting);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.comboRender);
            this.groupBox1.Controls.Add(this.drawGizmos);
            this.groupBox1.Controls.Add(this.showNormals);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.engineSetSize);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.engineHeight);
            this.groupBox1.Controls.Add(this.engineWidth);
            this.groupBox1.Controls.Add(this.comboBuffer);
            this.groupBox1.Controls.Add(this.wireframe);
            this.groupBox1.Controls.Add(this.showGrid);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(340, 138);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Engine";
            // 
            // showNormals
            // 
            this.showNormals.AutoSize = true;
            this.showNormals.Location = new System.Drawing.Point(6, 40);
            this.showNormals.Name = "showNormals";
            this.showNormals.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.showNormals.Size = new System.Drawing.Size(92, 17);
            this.showNormals.TabIndex = 9;
            this.showNormals.Text = "Show normals";
            this.showNormals.UseVisualStyleBackColor = true;
            this.showNormals.CheckedChanged += new System.EventHandler(this.showNormals_CheckedChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(161, 77);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Buffer";
            // 
            // engineSetSize
            // 
            this.engineSetSize.Location = new System.Drawing.Point(223, 111);
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
            this.label2.Location = new System.Drawing.Point(153, 116);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(14, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "X";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(39, 115);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Resolution";
            // 
            // engineHeight
            // 
            this.engineHeight.Location = new System.Drawing.Point(170, 112);
            this.engineHeight.Name = "engineHeight";
            this.engineHeight.Size = new System.Drawing.Size(46, 20);
            this.engineHeight.TabIndex = 4;
            // 
            // engineWidth
            // 
            this.engineWidth.Location = new System.Drawing.Point(102, 112);
            this.engineWidth.Name = "engineWidth";
            this.engineWidth.Size = new System.Drawing.Size(46, 20);
            this.engineWidth.TabIndex = 3;
            // 
            // comboBuffer
            // 
            this.comboBuffer.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBuffer.FormattingEnabled = true;
            this.comboBuffer.Items.AddRange(new object[] {
            "Render",
            "ZBuffer",
            "OBuffer"});
            this.comboBuffer.Location = new System.Drawing.Point(208, 73);
            this.comboBuffer.Name = "comboBuffer";
            this.comboBuffer.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.comboBuffer.Size = new System.Drawing.Size(117, 21);
            this.comboBuffer.TabIndex = 2;
            this.comboBuffer.SelectedIndexChanged += new System.EventHandler(this.ComboBox1_SelectedIndexChanged);
            // 
            // wireframe
            // 
            this.wireframe.AutoSize = true;
            this.wireframe.Location = new System.Drawing.Point(24, 19);
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
            this.showGrid.Location = new System.Drawing.Point(25, 61);
            this.showGrid.Name = "showGrid";
            this.showGrid.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.showGrid.Size = new System.Drawing.Size(73, 17);
            this.showGrid.TabIndex = 0;
            this.showGrid.Text = "Show grid";
            this.showGrid.UseVisualStyleBackColor = true;
            this.showGrid.CheckedChanged += new System.EventHandler(this.ShowGrid_CheckedChanged);
            // 
            // drawGizmos
            // 
            this.drawGizmos.AutoSize = true;
            this.drawGizmos.Checked = true;
            this.drawGizmos.CheckState = System.Windows.Forms.CheckState.Checked;
            this.drawGizmos.Location = new System.Drawing.Point(12, 82);
            this.drawGizmos.Name = "drawGizmos";
            this.drawGizmos.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.drawGizmos.Size = new System.Drawing.Size(86, 17);
            this.drawGizmos.TabIndex = 10;
            this.drawGizmos.Text = "Draw gizmos";
            this.drawGizmos.UseVisualStyleBackColor = true;
            this.drawGizmos.CheckedChanged += new System.EventHandler(this.drawGizmos_CheckedChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(161, 50);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(42, 13);
            this.label4.TabIndex = 12;
            this.label4.Text = "Render";
            // 
            // comboRender
            // 
            this.comboRender.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboRender.FormattingEnabled = true;
            this.comboRender.Items.AddRange(new object[] {
            "Full frame",
            "Interlaced",
            "Scanline"});
            this.comboRender.Location = new System.Drawing.Point(208, 46);
            this.comboRender.Name = "comboRender";
            this.comboRender.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.comboRender.Size = new System.Drawing.Size(117, 21);
            this.comboRender.TabIndex = 11;
            this.comboRender.SelectedIndexChanged += new System.EventHandler(this.comboRender_SelectedIndexChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(161, 23);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 13);
            this.label5.TabIndex = 14;
            this.label5.Text = "Lighting";
            // 
            // comboLighting
            // 
            this.comboLighting.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboLighting.FormattingEnabled = true;
            this.comboLighting.Items.AddRange(new object[] {
            "No lighting",
            "Vertex lighting",
            "Pixel lighting"});
            this.comboLighting.Location = new System.Drawing.Point(208, 19);
            this.comboLighting.Name = "comboLighting";
            this.comboLighting.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.comboLighting.Size = new System.Drawing.Size(117, 21);
            this.comboLighting.TabIndex = 13;
            this.comboLighting.SelectedIndexChanged += new System.EventHandler(this.comboLighting_SelectedIndexChanged);
            // 
            // EngineControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "EngineControl";
            this.Size = new System.Drawing.Size(340, 138);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox wireframe;
        private System.Windows.Forms.CheckBox showGrid;
        private System.Windows.Forms.ComboBox comboBuffer;
        private System.Windows.Forms.Button engineSetSize;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox engineHeight;
        private System.Windows.Forms.TextBox engineWidth;
        private System.Windows.Forms.CheckBox showNormals;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox comboLighting;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox comboRender;
        private System.Windows.Forms.CheckBox drawGizmos;
    }
}
