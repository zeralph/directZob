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
            this.zobScaleZ = new System.Windows.Forms.TextBox();
            this.zobScaleY = new System.Windows.Forms.TextBox();
            this.zobScaleX = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.zobRotZ = new System.Windows.Forms.TextBox();
            this.zobRotY = new System.Windows.Forms.TextBox();
            this.zobRotX = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.zobPosZ = new System.Windows.Forms.TextBox();
            this.zobPosY = new System.Windows.Forms.TextBox();
            this.zobPosX = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.zobName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.meshList = new System.Windows.Forms.ComboBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.meshList);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.zobScaleZ);
            this.groupBox1.Controls.Add(this.zobScaleY);
            this.groupBox1.Controls.Add(this.zobScaleX);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.zobRotZ);
            this.groupBox1.Controls.Add(this.zobRotY);
            this.groupBox1.Controls.Add(this.zobRotX);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.zobPosZ);
            this.groupBox1.Controls.Add(this.zobPosY);
            this.groupBox1.Controls.Add(this.zobPosX);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.zobName);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(249, 224);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "ZobObject";
            // 
            // zobScaleZ
            // 
            this.zobScaleZ.Location = new System.Drawing.Point(172, 156);
            this.zobScaleZ.Name = "zobScaleZ";
            this.zobScaleZ.Size = new System.Drawing.Size(50, 20);
            this.zobScaleZ.TabIndex = 21;
            this.zobScaleZ.Text = "100.00";
            this.zobScaleZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobScaleZ.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobScaleY
            // 
            this.zobScaleY.Location = new System.Drawing.Point(116, 156);
            this.zobScaleY.Name = "zobScaleY";
            this.zobScaleY.Size = new System.Drawing.Size(50, 20);
            this.zobScaleY.TabIndex = 20;
            this.zobScaleY.Text = "100.00";
            this.zobScaleY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobScaleY.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobScaleX
            // 
            this.zobScaleX.Location = new System.Drawing.Point(60, 156);
            this.zobScaleX.Name = "zobScaleX";
            this.zobScaleX.Size = new System.Drawing.Size(50, 20);
            this.zobScaleX.TabIndex = 19;
            this.zobScaleX.Text = "100.00";
            this.zobScaleX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobScaleX.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 159);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(34, 13);
            this.label4.TabIndex = 18;
            this.label4.Text = "Scale";
            // 
            // zobRotZ
            // 
            this.zobRotZ.Location = new System.Drawing.Point(172, 117);
            this.zobRotZ.Name = "zobRotZ";
            this.zobRotZ.Size = new System.Drawing.Size(50, 20);
            this.zobRotZ.TabIndex = 17;
            this.zobRotZ.Text = "100.00";
            this.zobRotZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobRotZ.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobRotY
            // 
            this.zobRotY.Location = new System.Drawing.Point(116, 117);
            this.zobRotY.Name = "zobRotY";
            this.zobRotY.Size = new System.Drawing.Size(50, 20);
            this.zobRotY.TabIndex = 16;
            this.zobRotY.Text = "100.00";
            this.zobRotY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobRotY.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobRotX
            // 
            this.zobRotX.Location = new System.Drawing.Point(60, 117);
            this.zobRotX.Name = "zobRotX";
            this.zobRotX.Size = new System.Drawing.Size(50, 20);
            this.zobRotX.TabIndex = 15;
            this.zobRotX.Text = "100.00";
            this.zobRotX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobRotX.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 120);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(47, 13);
            this.label3.TabIndex = 14;
            this.label3.Text = "Rotation";
            // 
            // zobPosZ
            // 
            this.zobPosZ.Location = new System.Drawing.Point(172, 78);
            this.zobPosZ.Name = "zobPosZ";
            this.zobPosZ.Size = new System.Drawing.Size(50, 20);
            this.zobPosZ.TabIndex = 13;
            this.zobPosZ.Text = "100.00";
            this.zobPosZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobPosZ.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobPosY
            // 
            this.zobPosY.Location = new System.Drawing.Point(116, 78);
            this.zobPosY.Name = "zobPosY";
            this.zobPosY.Size = new System.Drawing.Size(50, 20);
            this.zobPosY.TabIndex = 12;
            this.zobPosY.Text = "100.00";
            this.zobPosY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobPosY.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // zobPosX
            // 
            this.zobPosX.Location = new System.Drawing.Point(60, 78);
            this.zobPosX.Name = "zobPosX";
            this.zobPosX.Size = new System.Drawing.Size(50, 20);
            this.zobPosX.TabIndex = 11;
            this.zobPosX.Text = "100.00";
            this.zobPosX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ZobScaleZ_KeyPress);
            this.zobPosX.Leave += new System.EventHandler(this.UpdateFromTextBoxes);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 81);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "Position";
            // 
            // zobName
            // 
            this.zobName.Location = new System.Drawing.Point(60, 42);
            this.zobName.Name = "zobName";
            this.zobName.Size = new System.Drawing.Size(162, 20);
            this.zobName.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Name";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(7, 187);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(33, 13);
            this.label5.TabIndex = 22;
            this.label5.Text = "Mesh";
            // 
            // meshList
            // 
            this.meshList.FormattingEnabled = true;
            this.meshList.Location = new System.Drawing.Point(60, 183);
            this.meshList.Name = "meshList";
            this.meshList.Size = new System.Drawing.Size(162, 21);
            this.meshList.TabIndex = 23;
            this.meshList.SelectedValueChanged += new System.EventHandler(this.MeshList_SelectedValueChanged);
            // 
            // ZobObjectControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "ZobObjectControl";
            this.Size = new System.Drawing.Size(249, 224);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
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
    }
}
