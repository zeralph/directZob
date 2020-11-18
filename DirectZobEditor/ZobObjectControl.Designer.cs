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
            this.checkBoxLinkScale = new System.Windows.Forms.CheckBox();
            this.zobId = new System.Windows.Forms.TextBox();
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
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.checkBoxLinkScale);
            this.groupBox1.Controls.Add(this.zobId);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.zobName);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.zobPosX);
            this.groupBox1.Controls.Add(this.zobPosY);
            this.groupBox1.Controls.Add(this.zobPosZ);
            this.groupBox1.Controls.Add(this.zobScaleZ);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.zobScaleY);
            this.groupBox1.Controls.Add(this.zobRotX);
            this.groupBox1.Controls.Add(this.zobScaleX);
            this.groupBox1.Controls.Add(this.zobRotY);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.zobRotZ);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(340, 162);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "ZobObject";
            // 
            // checkBoxLinkScale
            // 
            this.checkBoxLinkScale.Appearance = System.Windows.Forms.Appearance.Button;
            this.checkBoxLinkScale.AutoSize = true;
            this.checkBoxLinkScale.Location = new System.Drawing.Point(231, 121);
            this.checkBoxLinkScale.Name = "checkBoxLinkScale";
            this.checkBoxLinkScale.Size = new System.Drawing.Size(23, 23);
            this.checkBoxLinkScale.TabIndex = 45;
            this.checkBoxLinkScale.Text = "L";
            this.checkBoxLinkScale.UseVisualStyleBackColor = true;
            // 
            // zobId
            // 
            this.zobId.Location = new System.Drawing.Point(63, 19);
            this.zobId.Name = "zobId";
            this.zobId.ReadOnly = true;
            this.zobId.Size = new System.Drawing.Size(162, 20);
            this.zobId.TabIndex = 44;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(22, 48);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 29;
            this.label1.Text = "Name";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(41, 22);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(16, 13);
            this.label7.TabIndex = 43;
            this.label7.Text = "Id";
            // 
            // zobName
            // 
            this.zobName.Location = new System.Drawing.Point(63, 45);
            this.zobName.Name = "zobName";
            this.zobName.Size = new System.Drawing.Size(162, 20);
            this.zobName.TabIndex = 30;
            this.zobName.Leave += new System.EventHandler(this.zobName_Leave);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 74);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 31;
            this.label2.Text = "Position";
            // 
            // zobPosX
            // 
            this.zobPosX.Location = new System.Drawing.Point(63, 71);
            this.zobPosX.Name = "zobPosX";
            this.zobPosX.Size = new System.Drawing.Size(50, 20);
            this.zobPosX.TabIndex = 32;
            this.zobPosX.Text = "100.00";
            this.zobPosX.Leave += new System.EventHandler(this.valueChanged);
            // 
            // zobPosY
            // 
            this.zobPosY.Location = new System.Drawing.Point(119, 71);
            this.zobPosY.Name = "zobPosY";
            this.zobPosY.Size = new System.Drawing.Size(50, 20);
            this.zobPosY.TabIndex = 33;
            this.zobPosY.Text = "100.00";
            this.zobPosY.Leave += new System.EventHandler(this.valueChanged);
            // 
            // zobPosZ
            // 
            this.zobPosZ.Location = new System.Drawing.Point(175, 71);
            this.zobPosZ.Name = "zobPosZ";
            this.zobPosZ.Size = new System.Drawing.Size(50, 20);
            this.zobPosZ.TabIndex = 34;
            this.zobPosZ.Text = "100.00";
            this.zobPosZ.Leave += new System.EventHandler(this.valueChanged);
            // 
            // zobScaleZ
            // 
            this.zobScaleZ.Location = new System.Drawing.Point(175, 123);
            this.zobScaleZ.Name = "zobScaleZ";
            this.zobScaleZ.Size = new System.Drawing.Size(50, 20);
            this.zobScaleZ.TabIndex = 42;
            this.zobScaleZ.Text = "100.00";
            this.zobScaleZ.Leave += new System.EventHandler(this.valueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(10, 100);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(47, 13);
            this.label3.TabIndex = 35;
            this.label3.Text = "Rotation";
            // 
            // zobScaleY
            // 
            this.zobScaleY.Location = new System.Drawing.Point(119, 123);
            this.zobScaleY.Name = "zobScaleY";
            this.zobScaleY.Size = new System.Drawing.Size(50, 20);
            this.zobScaleY.TabIndex = 41;
            this.zobScaleY.Text = "100.00";
            this.zobScaleY.Leave += new System.EventHandler(this.valueChanged);
            // 
            // zobRotX
            // 
            this.zobRotX.Location = new System.Drawing.Point(63, 97);
            this.zobRotX.Name = "zobRotX";
            this.zobRotX.Size = new System.Drawing.Size(50, 20);
            this.zobRotX.TabIndex = 36;
            this.zobRotX.Text = "100.00";
            this.zobRotX.Leave += new System.EventHandler(this.valueChanged);
            // 
            // zobScaleX
            // 
            this.zobScaleX.Location = new System.Drawing.Point(63, 123);
            this.zobScaleX.Name = "zobScaleX";
            this.zobScaleX.Size = new System.Drawing.Size(50, 20);
            this.zobScaleX.TabIndex = 40;
            this.zobScaleX.Text = "100.00";
            this.zobScaleX.Leave += new System.EventHandler(this.valueChanged);
            // 
            // zobRotY
            // 
            this.zobRotY.Location = new System.Drawing.Point(119, 97);
            this.zobRotY.Name = "zobRotY";
            this.zobRotY.Size = new System.Drawing.Size(50, 20);
            this.zobRotY.TabIndex = 37;
            this.zobRotY.Text = "100.00";
            this.zobRotY.Leave += new System.EventHandler(this.valueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(23, 126);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(34, 13);
            this.label4.TabIndex = 39;
            this.label4.Text = "Scale";
            // 
            // zobRotZ
            // 
            this.zobRotZ.Location = new System.Drawing.Point(175, 97);
            this.zobRotZ.Name = "zobRotZ";
            this.zobRotZ.Size = new System.Drawing.Size(50, 20);
            this.zobRotZ.TabIndex = 38;
            this.zobRotZ.Text = "100.00";
            this.zobRotZ.Leave += new System.EventHandler(this.valueChanged);
            // 
            // ZobObjectControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "ZobObjectControl";
            this.Size = new System.Drawing.Size(340, 162);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox checkBoxLinkScale;
        private System.Windows.Forms.TextBox zobId;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox zobName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox zobPosX;
        private System.Windows.Forms.TextBox zobPosY;
        private System.Windows.Forms.TextBox zobPosZ;
        private System.Windows.Forms.TextBox zobScaleZ;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox zobScaleY;
        private System.Windows.Forms.TextBox zobRotX;
        private System.Windows.Forms.TextBox zobScaleX;
        private System.Windows.Forms.TextBox zobRotY;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox zobRotZ;
    }
}
