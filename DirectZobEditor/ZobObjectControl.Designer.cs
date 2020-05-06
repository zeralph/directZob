namespace DirectZobEditor
{
    partial class ZobObjectControl
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.checkBoxLinkScale = new System.Windows.Forms.CheckBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.zobName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.zobPosX = new System.Windows.Forms.TextBox();
            this.zobPosY = new System.Windows.Forms.TextBox();
            this.zobPosZ = new System.Windows.Forms.TextBox();
            this.zobScaleZ = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.zobScaleY = new System.Windows.Forms.TextBox();
            this.zobRotX = new System.Windows.Forms.TextBox();
            this.zobScaleX = new System.Windows.Forms.TextBox();
            this.zobRotY = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.zobRotZ = new System.Windows.Forms.TextBox();
            this.tabZobObject = new System.Windows.Forms.TabControl();
            this.tabMesh = new System.Windows.Forms.TabPage();
            this.LightingModeBox = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.meshList = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.tabLight = new System.Windows.Forms.TabPage();
            this.button1 = new System.Windows.Forms.Button();
            this.lightBlue = new System.Windows.Forms.TextBox();
            this.lightGreen = new System.Windows.Forms.TextBox();
            this.lightRed = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.tabCamera = new System.Windows.Forms.TabPage();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tabZobObject.SuspendLayout();
            this.tabMesh.SuspendLayout();
            this.tabLight.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.splitContainer1);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(275, 500);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "ZobObject";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(3, 16);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.checkBoxLinkScale);
            this.splitContainer1.Panel1.Controls.Add(this.textBox1);
            this.splitContainer1.Panel1.Controls.Add(this.label1);
            this.splitContainer1.Panel1.Controls.Add(this.label7);
            this.splitContainer1.Panel1.Controls.Add(this.zobName);
            this.splitContainer1.Panel1.Controls.Add(this.label2);
            this.splitContainer1.Panel1.Controls.Add(this.zobPosX);
            this.splitContainer1.Panel1.Controls.Add(this.zobPosY);
            this.splitContainer1.Panel1.Controls.Add(this.zobPosZ);
            this.splitContainer1.Panel1.Controls.Add(this.zobScaleZ);
            this.splitContainer1.Panel1.Controls.Add(this.label3);
            this.splitContainer1.Panel1.Controls.Add(this.zobScaleY);
            this.splitContainer1.Panel1.Controls.Add(this.zobRotX);
            this.splitContainer1.Panel1.Controls.Add(this.zobScaleX);
            this.splitContainer1.Panel1.Controls.Add(this.zobRotY);
            this.splitContainer1.Panel1.Controls.Add(this.label4);
            this.splitContainer1.Panel1.Controls.Add(this.zobRotZ);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.tabZobObject);
            this.splitContainer1.Size = new System.Drawing.Size(269, 481);
            this.splitContainer1.SplitterDistance = 239;
            this.splitContainer1.TabIndex = 29;
            // 
            // checkBoxLinkScale
            // 
            this.checkBoxLinkScale.Appearance = System.Windows.Forms.Appearance.Button;
            this.checkBoxLinkScale.AutoSize = true;
            this.checkBoxLinkScale.Location = new System.Drawing.Point(225, 119);
            this.checkBoxLinkScale.Name = "checkBoxLinkScale";
            this.checkBoxLinkScale.Size = new System.Drawing.Size(23, 23);
            this.checkBoxLinkScale.TabIndex = 28;
            this.checkBoxLinkScale.Text = "L";
            this.checkBoxLinkScale.UseVisualStyleBackColor = true;
            this.checkBoxLinkScale.CheckedChanged += new System.EventHandler(this.checkBoxLinkScale_CheckedChanged);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(57, 17);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(162, 20);
            this.textBox1.TabIndex = 27;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 46);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Name";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(35, 20);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(16, 13);
            this.label7.TabIndex = 26;
            this.label7.Text = "Id";
            // 
            // zobName
            // 
            this.zobName.Location = new System.Drawing.Point(57, 43);
            this.zobName.Name = "zobName";
            this.zobName.Size = new System.Drawing.Size(162, 20);
            this.zobName.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 72);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "Position";
            // 
            // zobPosX
            // 
            this.zobPosX.Location = new System.Drawing.Point(57, 69);
            this.zobPosX.Name = "zobPosX";
            this.zobPosX.Size = new System.Drawing.Size(50, 20);
            this.zobPosX.TabIndex = 11;
            this.zobPosX.Text = "100.00";
            this.zobPosX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobPosX.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobPosY
            // 
            this.zobPosY.Location = new System.Drawing.Point(113, 69);
            this.zobPosY.Name = "zobPosY";
            this.zobPosY.Size = new System.Drawing.Size(50, 20);
            this.zobPosY.TabIndex = 12;
            this.zobPosY.Text = "100.00";
            this.zobPosY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobPosY.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobPosZ
            // 
            this.zobPosZ.Location = new System.Drawing.Point(169, 69);
            this.zobPosZ.Name = "zobPosZ";
            this.zobPosZ.Size = new System.Drawing.Size(50, 20);
            this.zobPosZ.TabIndex = 13;
            this.zobPosZ.Text = "100.00";
            this.zobPosZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobPosZ.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobScaleZ
            // 
            this.zobScaleZ.Location = new System.Drawing.Point(169, 121);
            this.zobScaleZ.Name = "zobScaleZ";
            this.zobScaleZ.Size = new System.Drawing.Size(50, 20);
            this.zobScaleZ.TabIndex = 21;
            this.zobScaleZ.Text = "100.00";
            this.zobScaleZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobScaleZ.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(4, 98);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(47, 13);
            this.label3.TabIndex = 14;
            this.label3.Text = "Rotation";
            // 
            // zobScaleY
            // 
            this.zobScaleY.Location = new System.Drawing.Point(113, 121);
            this.zobScaleY.Name = "zobScaleY";
            this.zobScaleY.Size = new System.Drawing.Size(50, 20);
            this.zobScaleY.TabIndex = 20;
            this.zobScaleY.Text = "100.00";
            this.zobScaleY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobScaleY.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobRotX
            // 
            this.zobRotX.Location = new System.Drawing.Point(57, 95);
            this.zobRotX.Name = "zobRotX";
            this.zobRotX.Size = new System.Drawing.Size(50, 20);
            this.zobRotX.TabIndex = 15;
            this.zobRotX.Text = "100.00";
            this.zobRotX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobRotX.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobScaleX
            // 
            this.zobScaleX.Location = new System.Drawing.Point(57, 121);
            this.zobScaleX.Name = "zobScaleX";
            this.zobScaleX.Size = new System.Drawing.Size(50, 20);
            this.zobScaleX.TabIndex = 19;
            this.zobScaleX.Text = "100.00";
            this.zobScaleX.TextChanged += new System.EventHandler(this.zobScaleX_TextChanged);
            this.zobScaleX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobScaleX.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobRotY
            // 
            this.zobRotY.Location = new System.Drawing.Point(113, 95);
            this.zobRotY.Name = "zobRotY";
            this.zobRotY.Size = new System.Drawing.Size(50, 20);
            this.zobRotY.TabIndex = 16;
            this.zobRotY.Text = "100.00";
            this.zobRotY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobRotY.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(17, 124);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(34, 13);
            this.label4.TabIndex = 18;
            this.label4.Text = "Scale";
            // 
            // zobRotZ
            // 
            this.zobRotZ.Location = new System.Drawing.Point(169, 95);
            this.zobRotZ.Name = "zobRotZ";
            this.zobRotZ.Size = new System.Drawing.Size(50, 20);
            this.zobRotZ.TabIndex = 17;
            this.zobRotZ.Text = "100.00";
            this.zobRotZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobRotZ.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // tabZobObject
            // 
            this.tabZobObject.Controls.Add(this.tabMesh);
            this.tabZobObject.Controls.Add(this.tabLight);
            this.tabZobObject.Controls.Add(this.tabCamera);
            this.tabZobObject.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabZobObject.Location = new System.Drawing.Point(0, 0);
            this.tabZobObject.Name = "tabZobObject";
            this.tabZobObject.SelectedIndex = 0;
            this.tabZobObject.Size = new System.Drawing.Size(269, 238);
            this.tabZobObject.TabIndex = 28;
            // 
            // tabMesh
            // 
            this.tabMesh.Controls.Add(this.LightingModeBox);
            this.tabMesh.Controls.Add(this.label5);
            this.tabMesh.Controls.Add(this.meshList);
            this.tabMesh.Controls.Add(this.label6);
            this.tabMesh.Location = new System.Drawing.Point(4, 22);
            this.tabMesh.Name = "tabMesh";
            this.tabMesh.Padding = new System.Windows.Forms.Padding(3);
            this.tabMesh.Size = new System.Drawing.Size(261, 212);
            this.tabMesh.TabIndex = 2;
            this.tabMesh.Text = "Mesh";
            this.tabMesh.UseVisualStyleBackColor = true;
            // 
            // LightingModeBox
            // 
            this.LightingModeBox.AutoCompleteCustomSource.AddRange(new string[] {
            "Flat shading",
            "Gouraud shading",
            "No shading"});
            this.LightingModeBox.FormattingEnabled = true;
            this.LightingModeBox.Items.AddRange(new object[] {
            "No lighting",
            "Flat shading",
            "Gouraud shading",
            "Phong shading",
            "Flat phong shading"});
            this.LightingModeBox.Location = new System.Drawing.Point(53, 63);
            this.LightingModeBox.Name = "LightingModeBox";
            this.LightingModeBox.Size = new System.Drawing.Size(162, 21);
            this.LightingModeBox.TabIndex = 25;
            this.LightingModeBox.SelectedIndexChanged += new System.EventHandler(this.LightingModeBox_SelectedIndexChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(14, 40);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(33, 13);
            this.label5.TabIndex = 22;
            this.label5.Text = "Mesh";
            // 
            // meshList
            // 
            this.meshList.FormattingEnabled = true;
            this.meshList.Location = new System.Drawing.Point(53, 36);
            this.meshList.Name = "meshList";
            this.meshList.Size = new System.Drawing.Size(162, 21);
            this.meshList.TabIndex = 23;
            this.meshList.SelectedValueChanged += new System.EventHandler(this.MeshList_SelectedValueChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(3, 67);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(44, 13);
            this.label6.TabIndex = 24;
            this.label6.Text = "Lighting";
            // 
            // tabLight
            // 
            this.tabLight.Controls.Add(this.button1);
            this.tabLight.Controls.Add(this.lightBlue);
            this.tabLight.Controls.Add(this.lightGreen);
            this.tabLight.Controls.Add(this.lightRed);
            this.tabLight.Controls.Add(this.label8);
            this.tabLight.Location = new System.Drawing.Point(4, 22);
            this.tabLight.Name = "tabLight";
            this.tabLight.Padding = new System.Windows.Forms.Padding(3);
            this.tabLight.Size = new System.Drawing.Size(261, 212);
            this.tabLight.TabIndex = 0;
            this.tabLight.Text = "Light";
            this.tabLight.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(221, 33);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(17, 23);
            this.button1.TabIndex = 33;
            this.button1.Text = "C";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // lightBlue
            // 
            this.lightBlue.Location = new System.Drawing.Point(165, 34);
            this.lightBlue.Name = "lightBlue";
            this.lightBlue.Size = new System.Drawing.Size(50, 20);
            this.lightBlue.TabIndex = 32;
            this.lightBlue.Text = "100.00";
            // 
            // lightGreen
            // 
            this.lightGreen.Location = new System.Drawing.Point(109, 34);
            this.lightGreen.Name = "lightGreen";
            this.lightGreen.Size = new System.Drawing.Size(50, 20);
            this.lightGreen.TabIndex = 31;
            this.lightGreen.Text = "100.00";
            // 
            // lightRed
            // 
            this.lightRed.Location = new System.Drawing.Point(53, 34);
            this.lightRed.Name = "lightRed";
            this.lightRed.Size = new System.Drawing.Size(50, 20);
            this.lightRed.TabIndex = 30;
            this.lightRed.Text = "100.00";
            this.lightRed.TextChanged += new System.EventHandler(this.lightRed_TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(13, 37);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(31, 13);
            this.label8.TabIndex = 29;
            this.label8.Text = "Color";
            // 
            // tabCamera
            // 
            this.tabCamera.Location = new System.Drawing.Point(4, 22);
            this.tabCamera.Name = "tabCamera";
            this.tabCamera.Padding = new System.Windows.Forms.Padding(3);
            this.tabCamera.Size = new System.Drawing.Size(261, 212);
            this.tabCamera.TabIndex = 1;
            this.tabCamera.Text = "Camera";
            this.tabCamera.UseVisualStyleBackColor = true;
            // 
            // ZobObjectControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "ZobObjectControl";
            this.Size = new System.Drawing.Size(275, 500);
            this.groupBox1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.tabZobObject.ResumeLayout(false);
            this.tabMesh.ResumeLayout(false);
            this.tabMesh.PerformLayout();
            this.tabLight.ResumeLayout(false);
            this.tabLight.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox zobName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox zobScaleZ;
        private System.Windows.Forms.TextBox zobScaleY;
        private System.Windows.Forms.TextBox zobScaleX;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox zobRotZ;
        private System.Windows.Forms.TextBox zobRotY;
        private System.Windows.Forms.TextBox zobRotX;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox zobPosZ;
        private System.Windows.Forms.TextBox zobPosY;
        private System.Windows.Forms.TextBox zobPosX;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox meshList;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox LightingModeBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TabControl tabZobObject;
        private System.Windows.Forms.TabPage tabLight;
        private System.Windows.Forms.TabPage tabCamera;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox lightBlue;
        private System.Windows.Forms.TextBox lightGreen;
        private System.Windows.Forms.TextBox lightRed;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.CheckBox checkBoxLinkScale;
        private System.Windows.Forms.TabPage tabMesh;
    }
}
